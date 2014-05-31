#include "bitjackcoinunits.h"

#include <QStringList>

BitjackcoinUnits::BitjackcoinUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<BitjackcoinUnits::Unit> BitjackcoinUnits::availableUnits()
{
    QList<BitjackcoinUnits::Unit> unitlist;
    unitlist.append(BJA);
    unitlist.append(mBJA);
    unitlist.append(uBJA);
    return unitlist;
}

bool BitjackcoinUnits::valid(int unit)
{
    switch(unit)
    {
    case BJA:
    case mBJA:
    case uBJA:
        return true;
    default:
        return false;
    }
}

QString BitjackcoinUnits::name(int unit)
{
    switch(unit)
    {
    case BJA: return QString("BJA");
    case mBJA: return QString("mBJA");
    case uBJA: return QString::fromUtf8("μBJA");
    default: return QString("???");
    }
}

QString BitjackcoinUnits::description(int unit)
{
    switch(unit)
    {
    case BJA: return QString("Bitjackcoins");
    case mBJA: return QString("Milli-Bitjackcoins (1 / 1,000)");
    case uBJA: return QString("Micro-Bitjackcoins (1 / 1,000,000)");
    default: return QString("???");
    }
}

mpq BitjackcoinUnits::factor(int unit)
{
    switch(unit)
    {
    case uBJA: return mpq("100/1");
    case mBJA: return mpq("100000/1");
    default:
    case BJA:  return mpq("100000000/1");
    }
}

int BitjackcoinUnits::amountDigits(int unit)
{
    switch(unit)
    {
    case BJA: return 8; // <100,000,000 (# digits, without commas)
    case mBJA: return 11; // <100,000,000,000
    case uBJA: return 14; // <100,000,000,000,000
    default: return 0;
    }
}

int BitjackcoinUnits::decimals(int unit)
{
    switch(unit)
    {
    case BJA: return 8;
    case mBJA: return 5;
    case uBJA: return 2;
    default: return 0;
    }
}

QString BitjackcoinUnits::format(int unit, const mpq& n, bool fPlus)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    mpq q = n * COIN / factor(unit);
    std::string str = FormatMoney(q, fPlus);
    int diff = 8 - decimals(unit);
    if(diff > 0)
        str.erase(str.length() - diff, diff);
    return QString::fromStdString(str);
}

QString BitjackcoinUnits::formatWithUnit(int unit, const mpq& amount, bool plussign)
{
    return format(unit, amount, plussign) + QString(" ") + name(unit);
}

bool BitjackcoinUnits::parse(int unit, const QString &value, mpq *val_out)
{
    mpq ret_value;
    if (!ParseMoney(value.toStdString(), ret_value))
        return false;
    if(val_out)
    {
        *val_out = ret_value * factor(unit) / COIN;
    }
    return true;
}

int BitjackcoinUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant BitjackcoinUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}
