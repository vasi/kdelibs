/*
    Copyright (c) 2009 Frederik Gladhorn <gladhorn@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef KNEWSTUFF3_ATTICA_PROVIDER_H
#define KNEWSTUFF3_ATTICA_PROVIDER_H

#include "provider.h"

namespace KNS3
{

    /**
     * @short KNewStuff Base Provider class.
     *
     * This class provides accessors for the provider object.
     * It should not be used directly by the application.
     * This class is the base class and will be instantiated for
     * static website providers.
     *
     * @author Jeremy Whiting <jpwhiting@kde.org>
     *
     * @internal
     */
    class KNEWSTUFF_EXPORT AtticaProvider: public Provider
    {
        Q_OBJECT
    public:
        
        AtticaProvider();

        ~AtticaProvider();

        
        virtual KTranslatable name() const;

        KUrl icon() const;

        virtual QStringList availableFeeds() const;

        /**
         * set the provider data xml, to initialize the provider
         */
        virtual bool setProviderXML(QDomElement & xmldata);

        /**
         * get the xml for the provider
         */
        virtual QDomElement providerXML() const;

        
        virtual void loadFeed(const QString& feedname, int page = 0);

        
       
    private:
        class AtticaProviderPrivate *d;
    };

}

#endif