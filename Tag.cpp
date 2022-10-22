#include "Tag.h"

Tag::Tag(QString label)
{
    this->setText(label);
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
