/*
 *   Copyright (C) %{CURRENT_YEAR} by %{AUTHOR} <%{EMAIL}>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .
 */

#include "%{APPNAMELC}part.h"

// KF headers
#include <KPluginFactory>
#include <KAboutData>
#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>

// Qt headers
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <QTextDocument>

K_PLUGIN_FACTORY(%{APPNAME}PartFactory, registerPlugin<%{APPNAME}Part>();)


%{APPNAME}Part::%{APPNAME}Part(QWidget* parentWidget, QObject* parent, const QVariantList& /*args*/)
    : KParts::ReadWritePart(parent)
{
    // set component data
    // the first arg must be the same as the subdirectory into which the part's rc file is installed
    KAboutData aboutData("%{APPNAMELC}part", i18n("%{APPNAME}Part"), QStringLiteral("%{VERSION}"));
    aboutData.addAuthor(i18n("%{AUTHOR}"), i18n("Author"), QStringLiteral("%{EMAIL}"));
    setComponentData(aboutData);

    // set internal UI
    // TODO: replace with your custom UI
    m_textEditWidget = new QTextEdit(parentWidget);
    setWidget(m_textEditWidget);

    // set KXMLUI resource file
    setXMLFile(QStringLiteral("%{APPNAMELC}partui.rc"));

    // setup actions
    setupActions();

    // starting with empty data model, not modified at begin
    // TODO: replace with your custom data model
    m_textDocument = new QTextDocument(this);
    m_textEditWidget->setDocument(m_textDocument);
    setModified(false);

    // set part read-write by default
    setReadWrite(true);
}

%{APPNAME}Part::~%{APPNAME}Part()
{
}

void %{APPNAME}Part::setupActions()
{
    m_saveAction = KStandardAction::save(this, &%{APPNAME}Part::fileSave, actionCollection());
    KStandardAction::saveAs(this, &%{APPNAME}Part::fileSaveAs, actionCollection());
}

void %{APPNAME}Part::setReadWrite(bool rw)
{
    // update internal UI
    m_textEditWidget->setReadOnly(!rw);

    // connect to modified state of data model
    if (rw) {
        connect(m_textDocument, &QTextDocument::modificationChanged,
                this,     &%{APPNAME}Part::setModified);
    } else {
        disconnect(m_textDocument, &QTextDocument::modificationChanged,
                   this,     &%{APPNAME}Part::setModified);
    }

    ReadWritePart::setReadWrite(rw);
}

void %{APPNAME}Part::setModified(bool modified)
{
    // update actions
    m_saveAction->setEnabled(modified);

    ReadWritePart::setModified(modified);
}

bool %{APPNAME}Part::openFile()
{
    QFile file(localFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    // TODO: replace with your custom file reading
    QTextStream stream(&file);
    QString text;
    while (!stream.atEnd()) {
        text += stream.readLine() + QLatin1Char('\n');
    }

    file.close();

    m_textDocument->setPlainText(text);

    return true;
}

bool %{APPNAME}Part::saveFile()
{
    // protect against wrong calls, as recommended in the ReadWritePart API dox
    if (!isReadWrite()) {
        return false;
    }

    QFile file(localFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    // TODO: replace with your custom file writing
    QTextStream stream(&file);
    stream << m_textDocument->toPlainText();

    file.close();

    // set current state in the data model as saved
    m_textDocument->setModified(false);

    return true;
}

void %{APPNAME}Part::fileSave()
{
    if (url().isValid()) {
        save();
    } else {
        fileSaveAs();
    }
}

void %{APPNAME}Part::fileSaveAs()
{
    const QUrl url = QFileDialog::getSaveFileUrl();
    if (url.isValid()) {
        saveAs(url);
    }
}

// needed for K_PLUGIN_FACTORY
#include <%{APPNAMELC}part.moc>