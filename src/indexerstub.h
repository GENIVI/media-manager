 /*
  * Copyright (C) 2014, Jaguar Land Rover
  *
  * Author: Jonatan Palsson <jonatan.palsson@pelagicore.com>
  *
  * This file is part of the GENIVI Media Manager Proof-of-Concept
  * For further information, see http://genivi.org/
  *
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this
  * file, You can obtain one at http://mozilla.org/MPL/2.0/.
  */

#ifndef INDEXERSTUB_H
#define INDEXERSTUB_H

#include <CommonAPI/CommonAPI.h>

#include "../src-gen/org/genivi/mediamanager/IndexerStubDefault.h"

#include "lms.h"

namespace MM = org::genivi::mediamanager;

/**
 * \brief Conversion layer between CommonAPI and internal API
 *
 * This class is undocumented, since it only provides a conversion
 * layer
 * between the auto-generated Franca functions (documented in the Indexer.fidl
 * file) and the Media Manager internal API, which is documented in the
 * LMSProvider class
 *
 * @see LMSProvider
 */
class IndexerStubImpl: public org::genivi::mediamanager::IndexerStubDefault {

public:
    IndexerStubImpl (LMSProvider *lms);
    virtual void getDatabasePath(std::string& output, MM::Indexer::IndexerError& e);
    virtual void startIndexing(MM::Indexer::IndexerError& e);
    virtual void stopIndexing(MM::Indexer::IndexerError& e);
    virtual const MM::Indexer::IndexerStatus& getIndexerStatusAttribute ();

private:
    LMSProvider *m_lms;
};

#endif /* INDEXERSTUB_H */
