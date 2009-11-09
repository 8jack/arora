/*
 * Copyright 2008 Christian Franke <cfchris6@ts2server.com>
 * Copyright 2008-2009 Benjamin C. Meyer <ben@meyerhome.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include "sourceviewer.h"

#include <qlayout.h>
#include <qmenubar.h>
#include <qnetworkcookie.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qpainter.h>
#include <qplaintextedit.h>
#include <qshortcut.h>
#include <qsettings.h>
#include <qwebframe.h>
#include <qwebpage.h>

#include "browserapplication.h"
#include "networkaccessmanager.h"
#include "plaintexteditsearch.h"
#include "sourcehighlighter.h"

LineNumber::LineNumber(TextEdit *parent)
        :QWidget(parent)
{
    editor = parent;
}

void LineNumber::paintEvent(QPaintEvent *event)
{
    editor->lineNumberPaintEvent(event);
}

void LineNumber::updateContents()
{
    update();
}


TextEdit::TextEdit(QString text, QWidget *parent)
        :QPlainTextEdit(text, parent)
{
    setLineWrapMode(QPlainTextEdit::WidgetWidth);
    setReadOnly(true);
    QFont font = QPlainTextEdit::font();
    font.setFamily(QLatin1String("Monospace"));
    setFont(font);
    setLineWidth(0);
    setFrameShape(QFrame::NoFrame);

    lineNumber = new LineNumber(this);

    connect(this, SIGNAL(updateRequest(QRect,int)), lineNumber, SLOT(updateContents()));

}


void TextEdit::lineNumberPaintEvent(QPaintEvent *e)
{
    QPainter p(lineNumber);
    p.fillRect(e->rect(), palette().window());
    p.setPen(QColor(Qt::darkGray));
    QFont font = p.font();
    p.setFont(font);

    QTextBlock block = firstVisibleBlock();
    while (block.isValid()) {
        qreal top = blockBoundingGeometry(block).translated(contentOffset()).top();
        /* 2 pixels for right margin */
        QRect rect(0, top, lineNumber->width() - 2, fontMetrics().height());

        if (top > height())
            break;

        p.drawText(rect, Qt::AlignRight, QString(QLatin1String("%1")).arg(block.blockNumber()));
        block = block.next();
    }
}


void TextEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    int bc = blockCount();
    QFontMetrics fm(QPlainTextEdit::font());
    QSize numberSize = fm.size(0, QString(QLatin1String("%1")).arg(bc));

    QRect rect = contentsRect();
    /* 2 pixels for left margin + 2 pixels for right margin = 4 */
    lineNumber->setGeometry(QRect(rect.top(), rect.left(), numberSize.width() + 4, rect.height()));
    setViewportMargins(numberSize.width() + 4, 0, 0, 0);
}


SourceViewer::SourceViewer(const QString &source, const QString &title,
                           const QUrl &url, QWidget *parent)
    : QDialog(parent)
    , m_edit(new TextEdit(tr("Loading..."), this))
    , m_highlighter(new SourceHighlighter(m_edit->document()))
    , m_plainTextEditSearch(new PlainTextEditSearch(m_edit, this))
    , m_layout(new QVBoxLayout(this))
    , m_menuBar(new QMenuBar(this))
    , m_editMenu(new QMenu(tr("&Edit"), m_menuBar))
    , m_findAction(new QAction(tr("&Find"), m_editMenu))
    , m_source(source)
{
    setWindowTitle(tr("Source of Page %1").arg(title));

    QSettings settings;
    settings.beginGroup(QLatin1String("SourceViewer"));
    QSize size = settings.value(QLatin1String("size"), QSize(640, 480)).toSize();
    resize(size);

    m_menuBar->addMenu(m_editMenu);
    m_editMenu->addAction(m_findAction);
    m_findAction->setShortcuts(QKeySequence::Find);
    connect(m_findAction, SIGNAL(triggered()),
            m_plainTextEditSearch, SLOT(showFind()));

    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addWidget(m_menuBar);
    m_layout->addWidget(m_plainTextEditSearch);
    m_layout->addWidget(m_edit);
    setLayout(m_layout);

    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    m_reply = BrowserApplication::networkAccessManager()->get(request);
    connect(m_reply, SIGNAL(finished()), this, SLOT(loadingFinished()));
    m_reply->setParent(this);
}

SourceViewer::~SourceViewer()
{
    QSettings settings;
    settings.beginGroup(QLatin1String("SourceViewer"));
    settings.setValue(QLatin1String("size"), size());
}

void SourceViewer::loadingFinished()
{
    QWebPage page;
    QByteArray response = m_reply->readAll();
    page.mainFrame()->setContent(response, QString(), m_reply->request().url());

    /* If original request was POST or a different problem is there, fall
       back to modified version of QWebFrame.toHtml() */
    if (page.mainFrame()->toHtml() != m_source)
        m_edit->setPlainText(m_source);
    else
        m_edit->setPlainText(QLatin1String(response));

    m_reply->close();
}
