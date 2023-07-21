#include <QCoreApplication>
#include <QDebug>
#include <QList>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QTextStream>

const QList<char> lowercaseAlphabet = {'a','b','c','d','e', 'f', 'g', 'h', 'i', 'j', 'k','l', 'm','n','o','p','q','r','s','t','u','v','w','x','y','z'};
const QList<char> uppercaseAlphabet = {'A','B','C','D','E', 'F', 'G', 'H', 'I', 'J', 'K','L', 'M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
const QList<char>  digits = {'0','1','2','3','4','5','6','7','8','9'};
const QList<char> symbols = {'!','@','#','$','%','^','&','*','(',')','-','_','=','+','{','}','[',']',';',':','\'','"',',','<','>','.','/','?','|','\\','`','~', ' '};

/*
 * ^.{8,25}$
 * ^                - beginning of new line
 * .                - any character
 * {8,25}           - min,max character count
 * $                - end of line
 *
 */
const QRegularExpression lengthRequirementsExpression("^.{8,25}$");
/*
 * (?=.*[a-z])                                                                   - at least one lowercase
 * (?=.*[A-Z])                                                                   - at least one uppercase
 * (?=.*[0-9])                                                                   - at least one digit
 * (?=.*[\\[\\$\\(\\)\\*\\+\\.\\?\\[\\]\\^\\{\\}\\|\\\\:;\"'%@!#&<>=/-_`,~ ])    - at least one special symbol
 *
 * Note \\S for symbols does not pass all cases, use explicit long string
 */
const QRegularExpression charRequirementsExpression("("
                                                    "(?=.*[a-z])"
                                                    "(?=.*[A-Z])"
                                                    "(?=.*[0-9])"
                                                    "(?=.*[\\[\\$\\(\\)\\*\\+\\.\\?\\[\\]\\^\\{\\}\\|\\\\:;\"'%@!#&<>=/-_`,~ ])"
                                                    ")");

bool passwordValidator(const QString password)
{

    // perform check
    bool matchOnChars = charRequirementsExpression.match(password).hasMatch();
    bool matchOnLength = lengthRequirementsExpression.match(password).hasMatch();
    bool result = matchOnChars && matchOnLength;

    // print results
    qInfo() << QString("Input password: %1, RESULT = %2, password matches characters: %3, password matches length: %4")
                   .arg(password)
                   .arg((result) ? "True" : "False")
                   .arg((matchOnChars) ? "True" : "False")
                   .arg((matchOnLength) ? "True" : "False");
    return result;
}

void getPasswordsFromCommandLine()
{
    qInfo() << "Please input a password";
    QTextStream s(stdin);
    QString password;
    do {
        // trimmed(): removes left & right whitespace - NOT midword spaces!
        password = s.readLine().trimmed();
        bool result = passwordValidator(password);
    } while (password != "quit");
}

QList<QString> passwordGenerator()
{
    QList<QString> passwordList;

    foreach(char symbol, symbols)
    {
        QString password;
        password.append(lowercaseAlphabet.at(QRandomGenerator::global()->bounded(0, lowercaseAlphabet.size())));
        password.append(uppercaseAlphabet.at(QRandomGenerator::global()->bounded(0, uppercaseAlphabet.size())));
        password.append(digits.at(QRandomGenerator::global()->bounded(0, digits.size())));
        password.append(symbol);
        while (password.size() < 25)
        {   password.append('a');
            if (password.size() >= 8)
                passwordList.append(password);
        }
    }

    qInfo() << "PasswordList size = " << passwordList.size() << ", Symbols size = " << symbols.size();
    return passwordList;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //    getPasswordsFromCommandLine();

    QList<QString> passwords = passwordGenerator();

    int successfulMatch = 0;
    int failedMatch = 0;
    foreach(QString pass, passwords)
    {
        if (passwordValidator(pass))
            successfulMatch++;
        else
            failedMatch++;
    }

    qInfo() << QString("Successful matches: %1, Failed matches: %2")
                   .arg(successfulMatch)
                   .arg(failedMatch);


    return a.exec();
}
