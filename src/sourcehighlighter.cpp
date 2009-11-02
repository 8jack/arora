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
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor(Qt::darkMagenta));
    keywordFormat.setFontWeight(QFont::Bold);
    setFormatFor(Keyword, keywordFormat);

    QTextCharFormat attributeFormat;
    attributeFormat.setFontWeight(QFont::Bold);
    setFormatFor(Attribute, attributeFormat);

    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor(Qt::lightGray));
    setFormatFor(Comment, commentFormat);

    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor(Qt::darkBlue));
    setFormatFor(String, stringFormat);

    QStringList tokens;
    //- HTML4.01 keywords
    tokens << QLatin1String("!DOCTYPE")
    << QLatin1String("a")
    << QLatin1String("abbr")
    << QLatin1String("acronym")
    << QLatin1String("address")
    << QLatin1String("applet")
    << QLatin1String("area")
    << QLatin1String("b")
    << QLatin1String("base")
    << QLatin1String("basefont")
    << QLatin1String("bdo")
    << QLatin1String("big")
    << QLatin1String("blockquote")
    << QLatin1String("body")
    << QLatin1String("br")
    << QLatin1String("button")
    << QLatin1String("caption")
    << QLatin1String("center")
    << QLatin1String("cite")
    << QLatin1String("code")
    << QLatin1String("col")
    << QLatin1String("colgroup")
    << QLatin1String("dd")
    << QLatin1String("del")
    << QLatin1String("dfn")
    << QLatin1String("dir")
    << QLatin1String("div")
    << QLatin1String("dl")
    << QLatin1String("dt")
    << QLatin1String("em")
    << QLatin1String("fieldset")
    << QLatin1String("font")
    << QLatin1String("form")
    << QLatin1String("frame")
    << QLatin1String("frameset")
    << QLatin1String("head")
    << QLatin1String("h1")
    << QLatin1String("h2")
    << QLatin1String("h3")
    << QLatin1String("h4")
    << QLatin1String("h5")
    << QLatin1String("h6")
    << QLatin1String("hr")
    << QLatin1String("html")
    << QLatin1String("i")
    << QLatin1String("iframe")
    << QLatin1String("img")
    << QLatin1String("input")
    << QLatin1String("ins")
    << QLatin1String("kbd")
    << QLatin1String("label")
    << QLatin1String("legend")
    << QLatin1String("li")
    << QLatin1String("link")
    << QLatin1String("map")
    << QLatin1String("menu")
    << QLatin1String("meta")
    << QLatin1String("noframes")
    << QLatin1String("noscript")
    << QLatin1String("object")
    << QLatin1String("ol")
    << QLatin1String("optgroup")
    << QLatin1String("option")
    << QLatin1String("p")
    << QLatin1String("param")
    << QLatin1String("pre")
    << QLatin1String("q")
    << QLatin1String("s")
    << QLatin1String("samp")
    << QLatin1String("script")
    << QLatin1String("select")
    << QLatin1String("small")
    << QLatin1String("span")
    << QLatin1String("strike")
    << QLatin1String("strong")
    << QLatin1String("style")
    << QLatin1String("sub")
    << QLatin1String("sup")
    << QLatin1String("table")
    << QLatin1String("tbody")
    << QLatin1String("td")
    << QLatin1String("textarea")
    << QLatin1String("tfoot")
    << QLatin1String("th")
    << QLatin1String("thead")
    << QLatin1String("title")
    << QLatin1String("tr")
    << QLatin1String("tt")
    << QLatin1String("u")
    << QLatin1String("ul")
    << QLatin1String("var");
    keywords = QSet<QString>::fromList(tokens);

    //- HTML Attributes
    tokens.clear();
    tokens << QLatin1String("abbr")
    << QLatin1String("accept-charset")
    << QLatin1String("accept")
    << QLatin1String("accesskey")
    << QLatin1String("action")
    << QLatin1String("align")
    << QLatin1String("alink")
    << QLatin1String("alt")
    << QLatin1String("archive")
    << QLatin1String("axis")
    << QLatin1String("background")
    << QLatin1String("bgcolor")
    << QLatin1String("border")
    << QLatin1String("cellspacing")
    << QLatin1String("char")
    << QLatin1String("charoff")
    << QLatin1String("charset")
    << QLatin1String("checked")
    << QLatin1String("cite")
    << QLatin1String("class")
    << QLatin1String("classid")
    << QLatin1String("clear")
    << QLatin1String("code")
    << QLatin1String("codebase")
    << QLatin1String("codetype")
    << QLatin1String("color")
    << QLatin1String("cols")
    << QLatin1String("colspan")
    << QLatin1String("compact")
    << QLatin1String("content")
    << QLatin1String("coords")
    << QLatin1String("data")
    << QLatin1String("datetime")
    << QLatin1String("declare")
    << QLatin1String("defer")
    << QLatin1String("dir")
    << QLatin1String("disabled")
    << QLatin1String("enctype")
    << QLatin1String("face")
    << QLatin1String("for")
    << QLatin1String("frame")
    << QLatin1String("frameborder")
    << QLatin1String("headers")
    << QLatin1String("height")
    << QLatin1String("href")
    << QLatin1String("hreflang")
    << QLatin1String("hspace")
    << QLatin1String("http-equiv")
    << QLatin1String("id")
    << QLatin1String("ismap")
    << QLatin1String("label")
    << QLatin1String("lang")
    << QLatin1String("language")
    << QLatin1String("link")
    << QLatin1String("longdesc")
    << QLatin1String("marginheight")
    << QLatin1String("marginwidth")
    << QLatin1String("maxlength")
    << QLatin1String("media")
    << QLatin1String("method")
    << QLatin1String("multiple")
    << QLatin1String("name")
    << QLatin1String("nohref")
    << QLatin1String("noresize")
    << QLatin1String("noshade")
    << QLatin1String("nowrap")
    << QLatin1String("object")
    << QLatin1String("onblur")
    << QLatin1String("onchange")
    << QLatin1String("onclick")
    << QLatin1String("ondblclick")
    << QLatin1String("onfocus")
    << QLatin1String("onkeydown")
    << QLatin1String("onkeypress")
    << QLatin1String("onkeyup")
    << QLatin1String("onload")
    << QLatin1String("onmousedown")
    << QLatin1String("onmousemove")
    << QLatin1String("onmouseout")
    << QLatin1String("onmouseover")
    << QLatin1String("onmouseup")
    << QLatin1String("onreset")
    << QLatin1String("onselect")
    << QLatin1String("onsubmit")
    << QLatin1String("onunload")
    << QLatin1String("profile")
    << QLatin1String("prompt")
    << QLatin1String("readonly")
    << QLatin1String("rel")
    << QLatin1String("rev")
    << QLatin1String("rows")
    << QLatin1String("rowspan")
    << QLatin1String("rules")
    << QLatin1String("scheme")
    << QLatin1String("scope")
    << QLatin1String("scrolling")
    << QLatin1String("selected")
    << QLatin1String("shape")
    << QLatin1String("size")
    << QLatin1String("span")
    << QLatin1String("src")
    << QLatin1String("standby")
    << QLatin1String("start")
    << QLatin1String("style")
    << QLatin1String("summary")
    << QLatin1String("tabindex")
    << QLatin1String("target")
    << QLatin1String("text")
    << QLatin1String("title")
    << QLatin1String("type")
    << QLatin1String("usemap")
    << QLatin1String("valign")
    << QLatin1String("value")
    << QLatin1String("valuetype")
    << QLatin1String("version")
    << QLatin1String("vlink")
    << QLatin1String("vspace")
    << QLatin1String("width");
    attributes = QSet<QString>::fromList(tokens);

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


bool SourceHighlighter::isKeyword(QString token)
{
    if (keywords.contains(token))
        return true;
    return false;
}

bool SourceHighlighter::isAttribute(QString token)
{
    if (attributes.contains(token))
        return true;
    return false;
}

void SourceHighlighter::hlTags(int start, int stop, QString text)
{
    /* Added char - for finding attribute "http-equiv" and "accepted-charset"
       Added char ! for finding !DOCTYPE */
    QString alphaNum = QLatin1String("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-!");

    QChar ch, prevCh;
    int state = Normal;
    int tokenStart = 0;
    for (int i=start; i<=stop; i++) {
        ch = text.at(i);
        if (state == Normal) {
            if ((!alphaNum.contains(prevCh)) && (alphaNum.contains(ch))) {
                state = InTag;
                tokenStart = i;
            }
        }
        else if (state == InTag) {
            if ((alphaNum.contains(prevCh)) && (!alphaNum.contains(ch))) {
                state = Normal;
                if (isKeyword(text.mid(tokenStart, i-tokenStart)))
                    setFormat(tokenStart, i-tokenStart, formats[Keyword]);
                else if (isAttribute(text.mid(tokenStart, i-tokenStart)))
                    setFormat(tokenStart, i-tokenStart, formats[Attribute]);
            }
        }
        prevCh = ch;
    }
}

void SourceHighlighter::highlightBlock(const QString &text)
{
    int state = previousBlockState();
    int len = text.length();
    int start = 0;
    int commentStart = 0;
    int stringStart = 0;
    int pos = 0;
    QRegExp regex;

    while (pos >= 0 && pos < len && len > 0) {
        switch (state) {
            default:
            case Normal:
                if (text.at(pos) == QLatin1Char('<')) {
                    if ( (len >= 4) && (pos <= (len - 4)) && (text.at(pos+1) == QLatin1Char('!')) &&
                         (text.at(pos+2) == QLatin1Char('-')) && (text.at(pos+3) == QLatin1Char('-')) ) {
                        commentStart = pos;
                        state = InComment;
                        break;
                    }
                    start = pos;
                    state = InTag;
                }
                break;
            case InTag:
                if (text.at(pos) == QLatin1Char('"')) {
                    hlTags(start, pos, text);
                    stringStart = pos;
                    state = InString;
                }
                else if (text.at(pos) == QLatin1Char('>')) {
                    hlTags(start, pos, text);
                    state = Normal;
                }
                break;
            case InComment:
                if ( (len >= 3) && (pos <= (len - 3)) && (text.at(pos) == QLatin1Char('-')) &&
                         (text.at(pos+1) == QLatin1Char('-')) && (text.at(pos+2) == QLatin1Char('>')) ) {
                    setFormat(commentStart, pos - commentStart + 3, formats[Comment]);
                    state = Normal;
                }
                break;
            case InString:
                if (text.at(pos) == QLatin1Char('"')) {
                    setFormat(stringStart, pos - stringStart + 1, formats[String]);
                    start = pos + 1;
                    state = InTag;
                }
                break;
        }
        ++pos;
    }
    if (state == InTag)
        hlTags(start, text.length() - 1, text);
    else if (state == InComment)
        setFormat(commentStart, text.length(), formats[Comment]);
    else if (state == InString)
        setFormat(stringStart, text.length(), formats[String]);
    setCurrentBlockState(state);
}
