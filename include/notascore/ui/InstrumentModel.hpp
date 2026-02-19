#pragma once

#ifdef NOTASCORE_ENABLE_QT

#include "notascore/ui/MainWindow.hpp"
#include <QAbstractListModel>
#include <vector>

namespace notascore::ui {

class InstrumentModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit InstrumentModel(std::vector<InstrumentDef>& library, QObject* parent = nullptr)
        : QAbstractListModel(parent), m_library(library) {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;
        return static_cast<int>(m_library.size());
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() < 0 || static_cast<std::size_t>(index.row()) >= m_library.size())
            return {};
        const auto& def = m_library[static_cast<std::size_t>(index.row())];
        if (role == Qt::DisplayRole) {
            return QString::fromStdString(def.name);
        }
        if (role == Qt::UserRole) {
            return QString::fromStdString(def.range);
        }
        return {};
    }

private:
    std::vector<InstrumentDef>& m_library;
};

} // namespace notascore::ui

#endif // NOTASCORE_ENABLE_QT
