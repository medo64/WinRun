#include <algorithm>
#include <QDebug>
#include <QLineEdit>
#include <QRect>
#include <QScreen>
#include "medo/about.h"
#include "medo/singleinstance.h"
#include "settings.h"
#include "state.h"
#include "visuals.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow() : QMainWindow(nullptr), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog);
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
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  wholeRect = screen->geometry();
    QRect  availRect = screen->availableGeometry();
    int spaceL = availRect.left() - wholeRect.left();
    int spaceR = wholeRect.right() - availRect.right();
    int spaceT = availRect.top() - wholeRect.top();
    int spaceB = wholeRect.bottom() - availRect.bottom();
    int space = std::max({spaceL, spaceR, spaceT, spaceB});
    if (space == spaceL) {  // taskbar on left (top, left)
        this->move(availRect.left(), availRect.top());
    } else if (space == spaceR) {  // taskbar on right (top, right)
        this->move(availRect.right() - (int)width(), availRect.top());
    } else if (space == spaceT) {  // taskbar on top (top, right)
        this->move(availRect.right() - (int)width(), availRect.top());
    } else {  // taskbar on bottom (bottom, right)
        this->move(availRect.right() - (int)width(), availRect.bottom() - (int)height());
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

    // load history
    QStringList list = State::history();
    for (QString item : list) {
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
    QCoreApplication::exit(0);
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
        QStringList history = State::history();
        history.prepend(ui->comboRun->currentText());
        history.removeDuplicates();
        while (history.size() > Settings::historyCount()) {
            history.removeLast();
        }
        State::setHistory(history);
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
    qDebug().noquote() << "Execute:" << text;


}
