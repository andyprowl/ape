#include "TableModel.hpp"

#include <QBrush>
#include <QFont>
#include <QTime>
#include <QTimer>

TableModel::TableModel(int const numOfRows, int const numOfColumns, QObject * const parent)
    : QAbstractTableModel{parent}
    , timer{new QTimer{this}}
    , contents{static_cast<std::size_t>(numOfRows), {static_cast<std::size_t>(numOfColumns), ""}}
{
    setupContents();

    setupTimer();
}

auto TableModel::rowCount(QModelIndex const & /*parent*/) const
    -> int
{
    return static_cast<int>(contents.size());
}

auto TableModel::columnCount(QModelIndex const & /*parent*/) const
    -> int
{
    return static_cast<int>(contents.front().size());
}

auto TableModel::headerData(int const section, Qt::Orientation orientation, int const role) const
    -> QVariant
{
    if ((role != Qt::DisplayRole) && (orientation == Qt::Horizontal))
    {
        return QVariant{};
    }

    return QString{"Section %1"}.arg(section); 
}

auto TableModel::data(QModelIndex const & index, int const role) const
    -> QVariant
{
    switch (role)
    {
        case Qt::DisplayRole:
        {
            return getCellText(index);
        }

        case Qt::ForegroundRole:
        {
            return getCellForeground(index);
        }

        case Qt::BackgroundRole:
        {
            return getCellBackground(index);
        }

        case Qt::FontRole:
        {
            return getCellFont(index);
        }

        case Qt::TextAlignmentRole:
        {
            return getCellAlignment(index);
        }

        default:
        {
            return QVariant{};
        }
    }
}

auto TableModel::setData(QModelIndex const & index, QVariant const & value, int role)
    -> bool
{
    if (role != Qt::EditRole)
    {
        return false;
    }

    if (!checkIndex(index))
    {
        return false;
    }

    contents[index.row()][index.column()] = value.toString();

    emit dataChanged(index, index, {Qt::DisplayRole});

    return true;
}

auto TableModel::flags(QModelIndex const & index) const
    -> Qt::ItemFlags
{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

auto TableModel::clear()
    -> void
{
    setupContents();

    auto const topLeftIndex = createIndex(0, 0);
    
    auto const bottomRightIndex = createIndex(rowCount() - 1, columnCount() - 1);

    emit dataChanged(topLeftIndex, bottomRightIndex, {Qt::DisplayRole});
}

auto TableModel::setupContents()
    -> void
{
    for (auto row = 0; row < rowCount(); ++row)
    {
        for (auto col = 0; col < columnCount(); ++col)
        {
            contents[row][col] = QString{"R%1,C%2"}.arg(row).arg(col);
        }
    }
}

auto TableModel::setupTimer()
    -> void
{
    timer->setInterval(1'000);

    timerConnection = timer->connect(timer, &QTimer::timeout, this, &TableModel::onTimeout);

    timer->start();
}

auto TableModel::onTimeout()
    -> void
{
    auto const topLeftIndex = createIndex(0, 0);

    emit dataChanged(topLeftIndex, topLeftIndex, {Qt::DisplayRole});
}

auto TableModel::getCellText(QModelIndex const & index) const
    -> QVariant
{
    if ((index.row() == 0) && (index.column() == 0))
    {
        return QTime::currentTime().toString();
    }

    return contents[index.row()][index.column()];
}

auto TableModel::getCellForeground(QModelIndex const & index) const
    -> QBrush
{
    if (((index.row() + index.column()) % 2) == 0)
    {
        return QBrush{Qt::blue};
    }
    else
    {
        return QBrush{Qt::red};
    }

}

auto TableModel::getCellBackground(QModelIndex const & index) const
    -> QBrush
{
    if ((index.row() == 0) && (index.column() == 0))
    {
        return QBrush{Qt::yellow};
    }

    return QBrush{};
}

auto TableModel::getCellFont(QModelIndex const & index) const
    -> QFont
{
    auto font = QFont{};

    if (((index.row() + index.column()) % 2) == 0)
    {
        font.setBold(true);
    }
    else
    {
        font.setStyle(QFont::Style::StyleItalic);
    }

    return font;
}

auto TableModel::getCellAlignment(QModelIndex const & index) const
    -> QVariant
{
    if (index.column() == 0)
    {
        return (Qt::AlignmentFlag::AlignHCenter + Qt::AlignmentFlag::AlignVCenter);
    }
    else
    {
        return (Qt::AlignmentFlag::AlignRight + Qt::AlignmentFlag::AlignVCenter);
    }
}
