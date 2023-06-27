#pragma once
#include <QString>


class Execute {

    public:

        static void execute(QString text);
        static bool tryExecuteDirectory(QString text);
        static bool tryExecuteFile(QString text);
        static bool tryExecuteCommand(QString text);
        static bool tryExecuteProtocol(QString path);
        static bool tryExecuteElse(QString text);

};
