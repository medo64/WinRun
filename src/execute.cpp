#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QStandardPaths>
#include <QStringList>
#include <QUrl>
#include "execute.h"


QString getPath(QString text){
    if (text.startsWith("/")) {
        return text;
    } else if (text.startsWith("~/")) {
        QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        if (homeDir.endsWith('/')) {
            return homeDir + text.mid(2);
        } else {
            return homeDir + '/' + text.mid(2);
        }
    } else if (text == "~") {
        QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        return homeDir;
    } else {
        return QString();
    }
}

void Execute::execute(QString text) {
    qDebug().noquote() << "Execute:" << text;

    if (tryExecuteDirectory(text)) { return; }
    if (tryExecuteFile(text)) { return; }
    if (tryExecuteCommand(text)) { return; }
}

bool Execute::tryExecuteDirectory(QString text) {
    QString path = getPath(text);
    if (path.isEmpty()) { return false; }
    QFileInfo info = QFileInfo(path);
    if (info.isDir()) {
        qDebug().noquote() << "Execute [Dir]:" << info.absoluteFilePath();
        return QDesktopServices::openUrl(QUrl::fromLocalFile(info.absoluteFilePath()));
    }
    return false;
}

bool Execute::tryExecuteFile(QString text) {
    QString path = getPath(text);
    if (path.isEmpty()) { return false; }
    QFileInfo info = QFileInfo(path);
    if (info.isFile()) {
        if (info.isExecutable()) {
            qDebug().noquote() << "Execute [File]:" << info.absoluteFilePath();
            return QProcess::startDetached(info.absoluteFilePath(), QStringList());
        } else {
            qDebug().noquote() << "Execute [File]:" << info.absoluteFilePath();
            return QDesktopServices::openUrl(QUrl::fromLocalFile(info.absoluteFilePath()));  // TODO: Error if it fails
        }
    }
    return false;
}

bool Execute::tryExecuteCommand(QString text) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString pathVariable = env.value("PATH");
    QStringList paths = pathVariable.split(":");

    for(const QString& path : qAsConst(paths)) {
        QDir dir(path);
        if(dir.exists(text)){
            QFileInfo info = QFileInfo(dir.filePath(text));
            if (info.isExecutable()) {
                qDebug().noquote() << "Execute [Command]:" << info.absoluteFilePath();
                return QProcess::startDetached(info.absoluteFilePath(), QStringList());
            }
        }
    }
    return false;
}
