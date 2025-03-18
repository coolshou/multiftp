#include "ftpmodel.h"

FtpModel::FtpModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    // Initialize column headers
    m_columnHeaders = {"Local File", "DIR", "Remote File", "status", "comment"};
}

QVariant FtpModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            return m_columnHeaders[section];
        } else {
            return QString("%1").arg(section + 1);
        }
    }
    return QVariant();
}

bool FtpModel::setHeaderData(int section,
                             Qt::Orientation orientation,
                             const QVariant &value,
                             int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        // m_columnHeaders[section] = value.toString();
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

int FtpModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    // if (!parent.isValid())
    //     return 0;

    // FIXME: Implement me!
    return m_data.size();
}

int FtpModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    // if (!parent.isValid())
    //     return 0;

    // FIXME: Implement me!
    return m_columnHeaders.size();
}

bool FtpModel::hasChildren(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    // FIXME: Implement me!
    return false;
}

bool FtpModel::canFetchMore(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    // FIXME: Implement me!
    return false;
}

void FtpModel::fetchMore(const QModelIndex &parent)
{
    Q_UNUSED(parent)
    // FIXME: Implement me!
}

QVariant FtpModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if((role==Qt::DisplayRole)&& !m_data.isEmpty()){
        if (index.row()>= m_data.size()){
            return QVariant();
        }
        if (index.column() >= m_data[index.row()].size()){
            return QVariant();
        }
        return m_data[index.row()][index.column()];
    }
    // switch (role)
    // {
    //     //case Qt::DecorationRole: // icon
    //     case Qt::DisplayRole:
    //     if (!m_data.isEmpty()){
    //         return m_data[index.row()][index.column()];
    //     }
    //     default:
    //         return QVariant();
    // }

    // FIXME: Implement me!
    return QVariant();
}

QModelIndex FtpModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex FtpModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}

bool FtpModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

bool FtpModel::setData(int row, int col, const QVariant &value)
{
    if (row<m_data.size()){
        if (col< m_data[row].size()){
            m_data[row][col] = value;
        }
    }
}

Qt::ItemFlags FtpModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

bool FtpModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool FtpModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return true;
}

void FtpModel::addData(const QVector<QVariant> &newRow)
{
    qDebug() << "newRow size:" << newRow.size() << " newRow:" << newRow;
    qDebug() << "m_data size:" << m_data.size() << " m_data:" << m_data;
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data.append(newRow);
    endInsertRows();
    qDebug() << "after insert m_data size:" << m_data.size() << " m_data:" << m_data;
}

bool FtpModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

bool FtpModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
    return true;
}

void FtpModel::clear()
{
    qDebug() << "TODO: FtpModel::clear";
}

void FtpModel::updateProgress(int id, qint64 bytesCurrent, qint64 bytesTotal)
{
    setData(id, Col::Status, QVariant(QString("%s/%s").arg(QString::number(bytesCurrent), QString::number(bytesTotal))));
}

void FtpModel::updateComment(int id, QString msg)
{
    setData(id, Col::Comment, QVariant(msg));
}
