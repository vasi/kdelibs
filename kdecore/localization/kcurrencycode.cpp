/*
    Copyright (c) 2009 John Layt <john@layt.net>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "kcurrencycode.h"

#include <QtCore/QSharedData>
#include <QtCore/QDate>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>

#include "kconfig.h"
#include "kconfiggroup.h"
#include "kglobal.h"
#include "klocale.h"
#include "kstandarddirs.h"
#include "kdebug.h"

class KCurrencyCodePrivate : public QSharedData
{
public:
    KCurrencyCodePrivate( const QString &isoCurrencyCode, const QString &language = QString() );
    KCurrencyCodePrivate( const QFileInfo &currencyCodeFile, const QString &language = QString() );
    virtual ~KCurrencyCodePrivate();

    void loadCurrency( const QFileInfo &currencyCodeFile, const QString &language );

    QString     m_currencyCodeIsoAlpha3;
    QString     m_currencyCodeIsoNumeric3;
    QString     m_currencyNameIso;
    QString     m_currencyNameDisplay;
    QStringList m_currencyUnitSymbols;
    QString     m_currencyUnitSymbolDefault;
    QString     m_currencyUnitSymbolUnambiguous;
    QString     m_currencyUnitSingular;
    QString     m_currencyUnitPlural;
    QString     m_currencySubunitSymbol;
    QString     m_currencySubunitSingular;
    QString     m_currencySubunitPlural;
    QDate       m_currencyIntroducedDate;
    QDate       m_currencySuspendedDate;
    QDate       m_currencyWithdrawnDate;
    int         m_currencySubunits;
    int         m_currencySubunitsPerUnit;
    bool        m_currencySubunitsInCirculation;
    int         m_currencyDecimalPlacesDisplay;
    QStringList m_currencyCountriesInUse;
};

KCurrencyCodePrivate::KCurrencyCodePrivate( const QString &isoCurrencyCode, const QString &language )
{
    QFileInfo file( KStandardDirs::locate( "locale", QString::fromLatin1( "currency/%1.desktop" ).arg( isoCurrencyCode.toLower() ) ) );

    loadCurrency( file, language );}

KCurrencyCodePrivate::KCurrencyCodePrivate( const QFileInfo &currencyCodeFile, const QString &language )
{
    loadCurrency( currencyCodeFile, language );
}

KCurrencyCodePrivate::~KCurrencyCodePrivate()
{
}

void KCurrencyCodePrivate::loadCurrency( const QFileInfo &currencyCodeFile, const QString &language )
{
    KConfig cgFile( currencyCodeFile.absoluteFilePath() );

    if ( language.isEmpty() ) {
        cgFile.setLocale( KGlobal::locale()->language() );
    } else {
        cgFile.setLocale( language );
    }

    KConfigGroup cg( &cgFile, "Currency Code" );

    m_currencyCodeIsoAlpha3         = cg.readEntry( "CurrencyCodeIsoAlpha3",         QString() );
    m_currencyCodeIsoNumeric3       = cg.readEntry( "CurrencyCodeIsoNumeric3",       QString() );
    m_currencyNameIso               = cg.readEntry( "CurrencyNameIso",               QString() );
    m_currencyNameDisplay           = cg.readEntry( "Name",                          QString() );
    m_currencyUnitSymbols           = cg.readEntry( "CurrencyUnitSymbols",           QStringList() );
    m_currencyUnitSymbolDefault     = cg.readEntry( "CurrencyUnitSymbolDefault",     QString() );
    m_currencyUnitSymbolUnambiguous = cg.readEntry( "CurrencyUnitSymbolUnambiguous", QString() );
    m_currencyUnitSingular          = cg.readEntry( "CurrencyUnitSingular",          QString() );
    m_currencyUnitPlural            = cg.readEntry( "CurrencyUnitPlural",            QString() );
    m_currencySubunitSymbol         = cg.readEntry( "CurrencySubunitSymbol",         QString() );
    m_currencySubunitSingular       = cg.readEntry( "CurrencySubunitSingular",       QString() );
    m_currencySubunitPlural         = cg.readEntry( "CurrencySubunitPlural",         QString() );
    m_currencyIntroducedDate        = cg.readEntry( "CurrencyIntroducedDate",        QDate() );
    m_currencySuspendedDate         = cg.readEntry( "CurrencySuspendedDate",         QDate() );
    m_currencyWithdrawnDate         = cg.readEntry( "CurrencyWithdrawnDate",         QDate() );
    m_currencySubunits              = cg.readEntry( "CurrencySubunits",              1 );
    m_currencySubunitsInCirculation = cg.readEntry( "CurrencySubunitsInCirculation", true );
    m_currencySubunitsPerUnit       = cg.readEntry( "CurrencySubunitsPerUnit",       100 );
    m_currencyDecimalPlacesDisplay  = cg.readEntry( "CurrencyDecimalPlacesDisplay",  2 );
    m_currencyCountriesInUse        = cg.readEntry( "CurrencyCountriesInUse",        QStringList() );
}

KCurrencyCode::KCurrencyCode( const QString &isoCurrencyCode, const QString &language )
              :d( new KCurrencyCodePrivate( isoCurrencyCode, language ) )
{
}

KCurrencyCode::KCurrencyCode( const QFileInfo &currencyCodeFile, const QString &language )
              :d( new KCurrencyCodePrivate( currencyCodeFile, language ) )
{
}

KCurrencyCode::KCurrencyCode( const KCurrencyCode &rhs )
              :d( rhs.d )
{
}

KCurrencyCode::~KCurrencyCode()
{
}

QString KCurrencyCode::isoCurrencyCode() const
{
    return d->m_currencyCodeIsoAlpha3;
}

QString KCurrencyCode::isoCurrencyCodeNumeric() const
{
    return d->m_currencyCodeIsoNumeric3;
}

QString KCurrencyCode::name() const
{
    return d->m_currencyNameDisplay;
}

QString KCurrencyCode::isoName() const
{
    return d->m_currencyNameIso;
}

KCurrencyCode::CurrencyStatus KCurrencyCode::status() const
{
    if ( dateWithdrawn() != QDate() ) {
        return ObsoleteCurrency;
    } else if ( dateSuspended() != QDate() ) {
        return SuspendedCurrency;
    } else {
        return ActiveCurrency;
    }
}

QDate KCurrencyCode::dateIntroduced() const
{
    return d->m_currencyIntroducedDate;
}

QDate KCurrencyCode::dateSuspended() const
{
    return d->m_currencySuspendedDate;
}

QDate KCurrencyCode::dateWithdrawn() const
{
    return d->m_currencyWithdrawnDate;
}

QStringList KCurrencyCode::symbolList() const
{
    return d->m_currencyUnitSymbols;
}

QString KCurrencyCode::defaultSymbol() const
{
    return d->m_currencyUnitSymbolDefault;
}

QString KCurrencyCode::unambiguousSymbol() const
{
    if ( d->m_currencyUnitSymbolUnambiguous.isEmpty() ) {
        return d->m_currencyUnitSymbolDefault;
    } else {
        return d->m_currencyUnitSymbolUnambiguous;
    }
}

bool KCurrencyCode::hasSubunits() const
{
    if ( d->m_currencySubunits > 0 ) {
        return true;
    } else {
        return false;
    }
}

bool KCurrencyCode::hasSubunitsInCirculation() const
{
    return ( hasSubunits() && d->m_currencySubunitsInCirculation );
}

QString KCurrencyCode::subunitSymbol() const
{
    return d->m_currencySubunitSymbol;
}

int KCurrencyCode::subunitsPerUnit() const
{
    return d->m_currencySubunitsPerUnit;
}

int KCurrencyCode::decimalPlaces() const
{
    return d->m_currencyDecimalPlacesDisplay;
}

QStringList KCurrencyCode::countriesUsingCurrency() const
{
    return d->m_currencyCountriesInUse;
}

bool KCurrencyCode::isValid() const
{
    return !d->m_currencyCodeIsoAlpha3.isEmpty();
}

bool KCurrencyCode::isValid( const QString &isoCurrencyCode, CurrencyStatusFlags currencyStatusFlags )
{
    KCurrencyCode test = KCurrencyCode( isoCurrencyCode, "us_EN" );
    return test.isValid() && ( currencyStatusFlags & test.status() );
}

QStringList KCurrencyCode::allCurrencyCodesList( KCurrencyCode::CurrencyStatusFlags currencyStatus )
{
    QStringList currencyCodes;

    const QStringList paths = KGlobal::dirs()->findAllResources( "locale", "currency/*.desktop" );

    foreach( QString path, paths )
    {
        QString code = path.mid( path.length()-11, 3 ).toUpper();

        if ( KCurrencyCode::isValid( code, currencyStatus ) ) {
            currencyCodes.append( code );
        }
    }

    return currencyCodes;
}

QString KCurrencyCode::currencyCodeToName( const QString &isoCurrencyCode )
{
    KCurrencyCode temp = KCurrencyCode( isoCurrencyCode );
    if ( temp.isValid() ) {
        return temp.name();
    } else {
        return QString();
    }
}