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
    if (tryExecuteProtocol(text)) { return; }
    if (tryExecuteElse(text)) { return; }
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

QString combineIntoNautilusArg(QString protocol, QString host, QString user, QString password, QString path) {
    QString args = protocol + "://";
    if (!user.isEmpty()) {
        if (!password.isEmpty()) {
            args += user + ":" + password + "@";
        } else {
            args += user + "@";
        }
    }
    args += host + "/" + path;
    return args;
}

bool tryExecuteFtp(QString host, QString user, QString password, QString path) {
    qDebug().noquote() << "Execute [Protocol Ftp]:" << host << path;
    QString arg1 = combineIntoNautilusArg("ftp", host, user, password, path);
    return QProcess::startDetached("/usr/bin/nautilus", QStringList { arg1 });  // TODO: support other file managers
}

bool tryExecuteSftp(QString host, QString user, QString password, QString path) {
    qDebug().noquote() << "Execute [Protocol Sftp]:" << host << path;
    QString arg1 = combineIntoNautilusArg("sftp", host, user, password, path);
    return QProcess::startDetached("/usr/bin/nautilus", QStringList { arg1 });  // TODO: support other file managers
}

bool tryExecuteSmb(QString host, QString user, QString password, QString path) {
    qDebug().noquote() << "Execute [Protocol Smb]:" << host << path;
    QString arg1 = combineIntoNautilusArg("smb", host, user, password, path);
    return QProcess::startDetached("/usr/bin/nautilus", QStringList { arg1 });  // TODO: support other file managers
}

bool tryExecuteSsh(QString host, QString user, QString password, QString path) {
    qDebug().noquote() << "Execute [Protocol Ssh]:" << host << path;
    QString arg1 = combineIntoNautilusArg("ssh", host, user, password, path);
    return QProcess::startDetached("/usr/bin/nautilus", QStringList { arg1 });  // TODO: support other file managers
}

QStringList splitIntoTwo(QString text, QString separator) {
    QStringList parts = text.split(separator, Qt::KeepEmptyParts);
    if (parts.length() < 2) { return parts; }
    QStringList outParts = { parts[0], parts.mid(1).join(separator) };
    return outParts;
}

bool Execute::tryExecuteProtocol(QString text) {
    if (text.startsWith("\\\\")) {
        text = "smb://" + text.mid(2).replace('\\', '/');
    }

    QStringList protoParts = splitIntoTwo(text, "://");
    if (protoParts.length() < 2) { return false; }  // cannot determine protocol
    QString protocol = protoParts[0];

    QString path;

    QStringList hostDefParts = splitIntoTwo(protoParts[1], "/");
    if (hostDefParts.length() == 2) {
        path = hostDefParts[1];
    }

    QString host;
    QString user;
    QString pass;
    QStringList userHostParts = splitIntoTwo(hostDefParts[0], "@");
    if (userHostParts.length() < 2) {
        host = hostDefParts[0];
    } else {
        host = userHostParts[1];
        QStringList userPassParts = splitIntoTwo(userHostParts[0], ":");
        user = userPassParts[0];
        if (userPassParts.length() == 2) {
            pass = userPassParts[1];
        }
    }

    if (protocol == "ftp") {
        return tryExecuteFtp(host, user, pass, path);
    } else if (protocol == "sftp") {
        return tryExecuteSftp(host, user, pass, path);
    } else if (protocol == "smb") {
        return tryExecuteSmb(host, user, pass, path);
    } else if (protocol == "ssh") {
        return tryExecuteSsh(host, user, pass, path);
    }
    return false;
}

bool Execute::tryExecuteElse(QString text) {
    qDebug().noquote() << "Execute [Else]:" << text;
    return QDesktopServices::openUrl(text);
}
