#ifndef MUSIC_DATA_VIEW_ITEM_HPP
#define MUSIC_DATA_VIEW_ITEM_HPP

#include <QListWidgetItem>

class MusicDataViewItem : public QListWidgetItem
{
public:
    MusicDataViewItem(const QString& text, const QString& path) 
        : QListWidgetItem(text), m_Path(path) 
    {}

    QString GetPath() const { return m_Path; }

private:
    QString m_Path;

};

#endif // MUSIC_DATA_VIEW_ITEM_HPP
