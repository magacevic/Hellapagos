#include <QJsonDocument>
#include <QFile>
#include "jsonserializer.h"

JSONSerializer::JSONSerializer()
{

}

void JSONSerializer::save(const Serializable &serializable, const QString &filepath, const QString &rootName)
{
    QVariant variant = serializable.toVariant();
    QJsonDocument doc = QJsonDocument::fromVariant(variant);

    if (doc.isNull()) {
        qWarning() << "Error: Failed to create JSON document from variant.";
        return;
    }

    QFile file(filepath);

    if (!file.open(QFile::WriteOnly)) {
        qWarning() << "Error: Unable to open file for writing:" << filepath << "-" << file.errorString();
        return;
    }

    QByteArray jsonData = doc.toJson(QJsonDocument::Indented);
    qint64 bytesWritten = file.write(jsonData);
    file.close();

    if (bytesWritten <= 0) {
        qWarning() << "Error: Failed to write JSON data to file:" << filepath;
    }
}


void JSONSerializer::load(Serializable &serializable, const QString &filepath)
{
    QFile file(filepath);

    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "Error: Unable to open file for reading:" << filepath;
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    if (jsonData.trimmed().isEmpty()) {
        qWarning() << "Error: File is empty:" << filepath;
        return;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Error: Failed to parse JSON -" << parseError.errorString();
        return;
    }

    serializable.fromVariant(doc.toVariant());
}
