#include <algorithm>
#include <QLineEdit>
#include <QRect>
#include <QScreen>
#include "medo/about.h"
#include "medo/singleinstance.h"
#include "execute.h"
#include "settings.h"
#include "state.h"
#include "visuals.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow() : QMainWindow(nullptr), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    this->setWindowIcon(Visuals::iconApp());

    // window size
    int minWidth = this->sizeHint().width();
    int defHeight = this->sizeHint().height();
    this->setMinimumSize(minWidth, defHeight);
    this->setMaximumSize(16777215, defHeight);
    int lastWidth = State::lastWidth();
    if (lastWidth < minWidth) { lastWidth = width(); }
    this->resize(lastWidth, defHeight);

    // window location
    int windowQuadrant = State::lastQuadrant();
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  wholeRect = screen->geometry();
    QRect  availRect = screen->availableGeometry();  // doesn't work in case of multiple monitors
    if (windowQuadrant == 0) {
        qDebug() << "Screen" << screen->name() << wholeRect << availRect;
        int spaceL = availRect.left() - wholeRect.left();
        int spaceR = wholeRect.right() - availRect.right();
        int spaceT = availRect.top() - wholeRect.top();
        int spaceB = wholeRect.bottom() - availRect.bottom();
        int space = std::max({spaceL, spaceR, spaceT, spaceB});
        if (space == spaceL) {  // taskbar on left (top, left)
            windowQuadrant = 7;
        } else if (space == spaceR) {  // taskbar on right (top, right)
            windowQuadrant = 9;
        } else if (space == spaceT) {  // taskbar on top (top, right)
            windowQuadrant = 9;
        } else {  // taskbar on bottom (bottom, right)
            windowQuadrant = 3;
        }
    }
    switch (windowQuadrant) {
        case 1:  // bottom-left
            this->move(availRect.left(), availRect.bottom() - (int)height());
            break;
        case 2:  // bottom-center
            this->move(availRect.left() + (availRect.width() - (int)width()) / 2, availRect.bottom() - (int)height());
            break;
        case 3:  // bottom-right
            this->move(availRect.right() - (int)width(), availRect.bottom() - (int)height());
            break;
        case 4:  // middle-left
            this->move(availRect.left(), availRect.top() + (availRect.height() - (int)height()) / 2);
            break;
        case 5:  // middle-center
            this->move(availRect.left() + (availRect.width() - (int)width()) / 2, availRect.top() + (availRect.height() - (int)height()) / 2);
            break;
        case 6:  // middle-right
            this->move(availRect.right() - (int)width(), availRect.top() + (availRect.height() - (int)height()) / 2);
            break;
        case 7:  // top-left
            this->move(availRect.left(), availRect.top());
            break;
        case 8:  // top-center
            this->move(availRect.left() + (availRect.width() - (int)width()) / 2, availRect.top());
            break;
        case 9:  // top-right
            this->move(availRect.right() - (int)width(), availRect.top());
            break;
    }

    // remove button images
    for (QAbstractButton* button : this->findChild<QDialogButtonBox*>()->buttons()) {
        button->setIcon(QIcon());
    }

    // single instance
    connect(SingleInstance::instance(), &SingleInstance::newInstanceDetected, this, &MainWindow::onShow);

    // hotkey setup (will remain active even when application is closed)
    _hotkey = new DConfHotkey("WinRun", this);
    _hotkey->registerHotkey(Settings::hotkey());

    // settings
    applySettings();

    // events
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &MainWindow::onDialogButtonClicked);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->comboRun, &QComboBox::currentTextChanged, [this](const QString& text) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!text.isEmpty());
    });
    ui->comboRun->installEventFilter(this);  // to intercept enter

    // load history
    QStringList list = State::history();
    for (const QString& item : qAsConst(list)) {
        ui->comboRun->addItem(item);
    }
    if (Settings::startEmpty()) {
        ui->comboRun->setCurrentIndex(-1);
    } else {
        ui->comboRun->lineEdit()->selectAll();
    }
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent* event) {
    Q_UNUSED(event);

    State::setLastWidth(width());

    // figure out the quadrant
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  rect = screen->geometry();
    int lineLeft = rect.left() + rect.width() / 3;
    int lineRight = rect.left() + rect.width() * 2 / 3;
    int lineTop = rect.top() + rect.height() / 3;
    int lineBottom = rect.top() + rect.height() * 2 / 3;
    int x = this->x() + this->width() / 2;
    int y = this->y() + this->height() / 2;
    qDebug() << "Screen" << rect
             << "L" << lineLeft << "R" << lineRight << "T" << lineTop << "B" << lineBottom
             << "-" << "X" << x << "Y" << y;
    if ((y <= lineTop) && (x <= lineLeft)) {
        State::setLastQuadrant(7);
    } else if ((y <= lineTop) && (x >= lineRight)) {
        State::setLastQuadrant(9);
    } else if (y <= lineTop) {
        State::setLastQuadrant(8);
    } else if ((y >= lineBottom) && (x <= lineLeft)) {
        State::setLastQuadrant(1);
    } else if ((y >= lineBottom) && (x >= lineRight)) {
        State::setLastQuadrant(3);
    } else if (y >= lineBottom) {
        State::setLastQuadrant(2);
    } else if (x <= lineLeft) {
        State::setLastQuadrant(4);
    } else if (x >= lineRight) {
        State::setLastQuadrant(6);
    } else {
        State::setLastQuadrant(5);
    }

    QCoreApplication::exit(0);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    bool isKeyEvent = (event->type() == QEvent::KeyPress);
    if (isKeyEvent) {
        if (obj == ui->comboRun) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                keyPressEvent(keyEvent);  // forward enter to main form
                return true; // Event handled, do not propagate further
            }
        }
    }
    return false;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    auto keyData = static_cast<uint>(event->key()) | event->modifiers();
    switch (keyData) {
        case Qt::Key_Escape:
        case Qt::ControlModifier | Qt::Key_F4:
        case Qt::ControlModifier | Qt::Key_W:
            this->close();
            break;

        case Qt::Key_Return:
            ui->buttonBox->button(QDialogButtonBox::Ok)->animateClick();
            break;

        case Qt::Key_F1:
            About::showDialog(this);
            break;

        default: QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::onShow() {
    this->setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    this->hide(); //workaround for Ubuntu
    this->show();
    this->raise(); //workaround for MacOS
    this->activateWindow(); //workaround for Windows
}

void MainWindow::onDialogButtonClicked(QAbstractButton* button) {
    if (button == ui->buttonBox->button(QDialogButtonBox::Ok)) {
        execute(ui->comboRun->currentText());
        this->close();
    } else {
        this->close();
    }
}

void MainWindow::applySettings(bool applyHotkey, bool applyForceDarkMode) {
    if (applyHotkey) {  // hotkey class will sort out unregister first
        _hotkey->registerHotkey(Settings::hotkey());
    }

    if (applyForceDarkMode) {
        bool isDark = Visuals::isOSInDarkMode() || Settings::forceDarkMode();
        Visuals::setupTheme(isDark);
    }
}

void MainWindow::execute(QString text) {
    QStringList history = State::history();
    history.prepend(ui->comboRun->currentText());
    history.removeDuplicates();
    while (history.size() > Settings::historyCount()) {
        history.removeLast();
    }
    State::setHistory(history);

    Execute::execute(text);
}
