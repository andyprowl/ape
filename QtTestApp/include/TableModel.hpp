#pragma once

#include <QAbstractTableModel>

#include <vector>

class QTimer;

class TableModel : public QAbstractTableModel
{

    Q_OBJECT;

public:

    TableModel(int numOfRows, int numOfColumns, QObject * parent = nullptr);

    auto rowCount(QModelIndex const & parent = QModelIndex{}) const
        -> int override;

    auto columnCount(QModelIndex const & parent = QModelIndex{}) const
        -> int override;

    auto headerData(int section, Qt::Orientation orientation, int role) const
        -> QVariant override;

    auto data(QModelIndex const & index, int role = Qt::DisplayRole) const
        -> QVariant override;

    auto setData(QModelIndex const & index, QVariant const & value, int role)
        -> bool override;

    auto flags(QModelIndex const & index) const
        -> Qt::ItemFlags override;

    auto clear()
        -> void;

private:

    auto setupContents()
        -> void;

    auto setupTimer()
        -> void;

    auto onTimeout()
        -> void;
    
    auto getCellText(QModelIndex const & index) const
        -> QVariant;

    auto getCellForeground(QModelIndex const & index) const
        -> QBrush;

    auto getCellBackground(QModelIndex const & index) const
        -> QBrush;

    auto getCellFont(QModelIndex const & index) const
        -> QFont;

    auto getCellAlignment(QModelIndex const & index) const
        -> QVariant;

private:

    std::vector<std::vector<QString>> contents;

    QTimer * timer;

};
