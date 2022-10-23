#include "Tag.h"

Tag::Tag(QString label)
{
    this->setText(label);
    this->setAlignment(Qt::AlignHCenter);
    this->setMinimumWidth(31);
    this->setStyleSheet(
                "   background-color : #653133;"
                "   color : #d17579;"
                "   padding : 1px 5px 3px 5px;"
                "   border-radius:12px;"
                "   border: 2px solid #653133;"
    );
}

Tag::Tag(Tag* tag) {
    this->setText(tag->text());
    this->setAlignment(Qt::AlignHCenter);
    this->setMinimumWidth(31);
    this->setStyleSheet(
                "   background-color : #653133;"
                "   color : #d17579;"
                "   padding : 1px 5px 3px 5px;"
                "   border-radius:12px;"
                "   border: 2px solid #653133;"
    );
}

QString Tag::getSavedTag() {
    return m_savedTag;
}

void Tag::setSavedTag(QString tag) {
    m_savedTag = tag;
}

void Tag::mousePressEvent(QMouseEvent* event) {
    emit clicked(this);
}

void Tag::enterEvent(QEnterEvent *event) {
    emit mouseEnter(this);
}

void Tag::leaveEvent(QEvent *event) {
    emit mouseLeave(this);
}
