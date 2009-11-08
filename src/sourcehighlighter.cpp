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

#include "sourcehighlighter.h"

#include <qpalette.h>

SourceHighlighter::SourceHighlighter(QTextDocument *document)
    : QSyntaxHighlighter(document)
{
    QTextCharFormat attributeFormat;
    attributeFormat.setFontWeight(QFont::Bold);
    setFormatFor(Attribute, attributeFormat);

    QTextCharFormat tagFormat;
    tagFormat.setForeground(QColor(Qt::darkMagenta));
    tagFormat.setFontWeight(QFont::Bold);
    setFormatFor(Tag, tagFormat);

    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor(Qt::lightGray));
    setFormatFor(Comment, commentFormat);

    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor(Qt::darkBlue));
    setFormatFor(String, stringFormat);

    QTextCharFormat doctypeFormat;
    doctypeFormat.setForeground(QColor(Qt::darkGreen));
    doctypeFormat.setFontWeight(QFont::Bold);
    setFormatFor(Doctype, doctypeFormat);
}

QTextCharFormat SourceHighlighter::getFormatFor(Construct construct)
{
    return formats[construct];
}

void SourceHighlighter::setFormatFor(Construct construct,
                         QTextCharFormat &format)
{
    formats[construct] = format;
}

void SourceHighlighter::highlightBlock(const QString &text)
{
    int state = previousBlockState();
    int len = text.length();
    int pos = 0;
    int regIndex = 0;
    int attributeStart = 0;
    int stringStart = 0;
    int commentStart = 0;
    int doctypeStart = 0;
    QRegExp regex;

    regex = QRegExp(QLatin1String("^</?[A-Za-z_:][\\w.:_-]*"));

    while (pos >= 0 && pos < len && len > 0) {
        switch (state) {
            default:
            case Normal:
                if (text.at(pos) == QLatin1Char('<')) {
                    if (text.mid(pos, 4) == QLatin1String("<!--")) {
                        commentStart = pos;
                        state = InComment;
                    }
                    else if (text.mid(pos, 9) == QLatin1String("<!DOCTYPE")) {
                        doctypeStart = pos;
                        state = InDoctype;
                    }
                    else state = InTag;
                }
                else ++pos;
                break;

            case InDoctype:
                if (text.at(pos) == QLatin1Char('>')) {
                    setFormat(doctypeStart, pos - doctypeStart + 1, formats[Doctype]);
                    state = Normal;
                }
                ++pos;
                break;

            case InComment:
                if (text.at(pos) == QLatin1Char('-')) {
                    if (text.mid(pos, 3) == QLatin1String("-->")) {
                        setFormat(commentStart, pos - commentStart + 3, formats[Comment]);
                        state = Normal;
                    }
                }
                ++pos;
                break;

            case InTag:
                regIndex = regex.indexIn(text, pos, QRegExp::CaretAtOffset);
                if (regIndex == pos) {
                    setFormat(regIndex + 1, regex.matchedLength() - 1, formats[Tag]);
                    pos += regex.matchedLength();
                    state = InAttribute;
                    attributeStart = pos;
                }
                else ++pos;
                break;

            case InAttribute:
                if (text.at(pos) == QLatin1Char('"')) {
                    setFormat(attributeStart, pos - attributeStart, formats[Attribute]);
                    stringStart = pos;
                    state = InString;
                }
                else if (text.at(pos) == QLatin1Char('>')) {
                    setFormat(attributeStart, pos - attributeStart, formats[Attribute]);
                    state = Normal;
                }
                ++pos;
                break;

            case InString:
                if (text.at(pos) == QLatin1Char('"')) {
                    setFormat(stringStart, pos - stringStart, formats[String]);
                    attributeStart = pos + 1;
                    state = InAttribute;
                }
                ++pos;
                break;

        }
    }
    /* Handle multi line string/comment */
    if (state == InString)
        setFormat(stringStart, len - stringStart, formats[String]);
    else if (state == InComment)
        setFormat(commentStart, len - commentStart, formats[Comment]);

    setCurrentBlockState(state);
}
