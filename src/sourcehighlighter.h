/*
 * Copyright 2008 Christian Franke <cfchris6@ts2server.com>
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

#ifndef SOURCEHIGHLIGHTER_H
#define SOURCEHIGHLIGHTER_H

#include <qsyntaxhighlighter.h>
#include <qset.h>

class SourceHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    enum Construct {
        Keyword,
        Comment,
        String,
        Attribute,
        LastConstruct = Attribute
    };
    SourceHighlighter(QTextDocument *document);

    QTextCharFormat getFormatFor(Construct construct);
    void setFormatFor(Construct construct, QTextCharFormat &format);

protected:
    enum State {
        Normal = -1,
        InComment,
        InTag,
        InString
    };
    void highlightBlock(const QString &text);
    void hlTags(int start, int stop, QString text);
    bool isKeyword(QString token);
    bool isAttribute(QString token);

private:
    QTextCharFormat formats[LastConstruct + 1];
    QSet<QString> keywords;
    QSet<QString> attributes;
};

#endif
