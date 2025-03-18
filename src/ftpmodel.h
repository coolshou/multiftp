#ifndef FTPMODEL_H
#define FTPMODEL_H

#include <QAbstractTableModel>
#include <QVector>

class FtpModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Col {
        Localfile,
        Dir,
        RemoteFile,
        Status,
        Comment
    };
    explicit FtpModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section,
                       Qt::Orientation orientation,
                       const QVariant &value,
                       int role = Qt::EditRole) override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Fetch data dynamically:
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool setData(int row, int col, const QVariant &value);
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    void addData(const QVector<QVariant> &newRow);
    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    void clear();
public slots:
    void updateProgress(int id, qint64 bytesCurrent, qint64 bytesTotal);
    void updateComment(int id, QString msg);
private:
    QVector<QString> m_columnHeaders;
    QVector<QVector<QVariant>> m_data;
};

#endif // FTPMODEL_H
