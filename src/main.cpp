#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QTextStream>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

#include "httplib.h"

QString generateBreadcrumbs(const QString &relativePath) {
    QStringList parts = relativePath.split('/', Qt::SkipEmptyParts);
    QString html = "<div><a href='/'>Root</a>";
    QString cumulativePath;
    for (const QString &part : parts) {
        cumulativePath += "/" + part;
        html += " / <a href='/browse" + cumulativePath + "'>" + part + "</a>";
    }
    html += "</div><hr>";
    return html;
}

QString generateHtmlTree(const QDir &dir, const QString &relativePath = "", const QString &filter = "") {
    QString html = "<ul>";
    QFileInfoList entries = QDir(dir.filePath(relativePath)).entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

    for (const QFileInfo &entry : entries) {
        QString name = entry.fileName();
        QString path = relativePath.isEmpty() ? name : relativePath + "/" + name;

        if (!filter.isEmpty() && !name.contains(filter, Qt::CaseInsensitive))
            continue;

        if (entry.isDir()) {
            html += "<li>";
            html += "📁 <span class='folder' onclick='toggle(this)'>" + name + "</span>";
            html += "<div class='nested'>" + generateHtmlTree(dir, path, filter) + "</div>";
            html += "</li>";
        } else {
            html += "<li>📄 <a href='/download/" + path + "'>" + name + "</a></li>";
        }
    }

    html += "</ul>";
    return html;
}

QString buildPage(const QDir &dir, const QString &relativePath, const QString &filter = "") {
    QString html = "<!DOCTYPE html><html><head><meta charset='utf-8'><title>File Server</title><style>"
                   "body { font-family: sans-serif; padding: 2em; }"
                   "ul { list-style-type: none; padding-left: 1em; }"
                   "li { margin: 4px 0; }"
                   ".folder { cursor: pointer; font-weight: bold; }"
                   ".nested { display: none; margin-left: 1em; }"
                   ".nested.active { display: block; }"
                   "</style>"
                   "<script>"
                   "function toggle(el) {"
                   "  var nested = el.nextElementSibling;"
                   "  if (nested) nested.classList.toggle('active');"
                   "}"
                   "</script></head><body>";

    html += "<h2>Shared Files & Folders</h2>";
    html += "<form method='get' action=''><input type='text' name='filter' placeholder='Search...' value='" + filter.toHtmlEscaped() + "' />";
    html += "<input type='submit' value='Search'></form><br>";
    html += generateBreadcrumbs(relativePath);
    html += generateHtmlTree(dir, relativePath,filter);
    html += "</body></html>";
    return html;
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Qt File Server");
    QCoreApplication::setApplicationVersion("2.3");

    QCommandLineParser parser;
    parser.setApplicationDescription("Qt5 + cpp-httplib File Server with Tree View + Folder Toggle");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption portOption({"p", "port"}, "Port number (default: 8080)", "port", "8080");
    QCommandLineOption dirOption({"d", "directory"}, "Shared directory path", "directory");

    parser.addOption(portOption);
    parser.addOption(dirOption);
    parser.process(app);

    QString sharedFolder = parser.value(dirOption);
    if (sharedFolder.isEmpty()) {
        qCritical("Error: --directory must be specified.");
        return 1;
    }

    QDir dir(sharedFolder);
    if (!dir.exists()) {
        qCritical("Directory does not exist: %s", qUtf8Printable(sharedFolder));
        return 1;
    }

    int port = parser.value(portOption).toInt();
    if (port <= 0 || port > 65535) {
        qCritical("Invalid port number: %d", port);
        return 1;
    }

    httplib::Server svr;

    svr.Get("/", [&dir](const httplib::Request &req, httplib::Response &res) {
        QString filter;
        if (req.has_param("filter")) {
            filter = QString::fromStdString(req.get_param_value("filter"));
        }
        QString html = buildPage(dir, "", filter);
        res.set_content(html.toUtf8().constData(), "text/html");
    });

    svr.Get(R"(/browse/(.+))", [&dir](const httplib::Request &req, httplib::Response &res) {
        QString relPath = QString::fromStdString(req.matches[1]);
        QString filter;
        if (req.has_param("filter")) {
            filter = QString::fromStdString(req.get_param_value("filter"));
        }
        QString html = buildPage(dir, relPath, filter);
        res.set_content(html.toUtf8().constData(), "text/html");
    });

    svr.Get(R"(/download/(.+))", [&dir](const httplib::Request &req, httplib::Response &res) {
        QString relPath = QString::fromStdString(req.matches[1]);
        QFile file(dir.filePath(relPath));

        if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
            res.status = 404;
            res.set_content("404 - File not found", "text/plain");
            return;
        }

        QByteArray content = file.readAll();
        res.set_content(content.constData(), content.size(), "application/octet-stream");
        res.set_header("Content-Disposition",
                       QString("attachment; filename=\"%1\"").arg(QFileInfo(file).fileName()).toUtf8().constData());
    });

    qDebug() << "Serving directory:" << sharedFolder;
    qDebug() << "Server running at http://0.0.0.0:" << port;

    svr.listen("0.0.0.0", port);

    return 0;
}
