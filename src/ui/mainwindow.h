#pragma once

#include <QApplication>
#include <QCloseEvent>
#include <QFutureWatcher>
#include <QMainWindow>
#include <QPushButton>
#include <QToolButton>
#include <QSystemTrayIcon>
#include "medo/dconfHotkey.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        explicit MainWindow();
        ~MainWindow();

    protected:
        void closeEvent(QCloseEvent* event) override;
        bool eventFilter(QObject* obj, QEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;

    private:
        Ui::MainWindow* ui;
        DConfHotkey* _hotkey = nullptr;
        void applySettings(bool applyHotkey = false, bool applyForceDarkMode = true);
        void execute(QString text);

    private slots:
        void onShow();
        void onDialogButtonClicked(QAbstractButton* button);

};
