/*
 * @file opencog/embodiment/Control/OperationalAvatarController/PsiFeelingUpdaterAgent.h
 *
 * @author Zhenhua Cai <czhedu@gmail.com>
 * @date 2011-02-06
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#ifndef PSIFEELINGUPDATERAGENT_H
#define PSIFEELINGUPDATERAGENT_H

#include <opencog/server/Agent.h>
#include <opencog/atomspace/AtomSpace.h>

namespace OperationalAvatarController
{

/**
 * @class
 *
 * @brief MindAgent for updating feelings
 *
 * @note Each feeling in AtomSpace is represented as below:
 *
 *       EvaluationLink
 *           PredicateNode "feelingName"
 *           ListLink
 *               petHandle
 */
class PsiFeelingUpdaterAgent : public opencog::Agent
{
private:

    /**
     *  Helper class that stores meta data of a feeling 
     */
    class FeelingMeta
    {
    public:

        std::string updaterName; // The schema name that updates the feeling
        Handle evaluationLink;   // Handle to EvaluationLink that holds the feeling 
        double updatedValue;     // The updated value after calling the feeling updater  
        bool bUpdated;           // Indicate if the value of the Feeling has been updated

        void init (const std::string & updaterName, Handle evaluationLink) {
            this->updaterName = updaterName;
            this->evaluationLink = evaluationLink; 
            this->updatedValue = 0;
            this->bUpdated = false;
        }
    }; // class

    unsigned long cycleCount;  // Indicates this mind agent has been executed how many times

    bool bInitialized;         // Indicate whether this mind agent has been Initialized

    std::map <std::string, FeelingMeta> feelingMetaMap;  // Feeling - Meta data map 

    /**
     * Initialize feelingMetaMap etc.
     */
    void init(opencog::CogServer * server);

    /**
     * Get corresponding EvaluationLink of the feeling 
     */
    Handle getFeelingEvaluationLink(opencog::CogServer * server, const std::string feelingName, Handle petHandle); 

    /**
     * Run updaters (combo scripts)
     */
    void runUpdaters(opencog::CogServer * server);

    /**
     * Set updated values to AtomSpace
     */
    void setUpdatedValues(opencog::CogServer * server);

    /**
     * Send updated values to the virtual world where the pet lives
     */
    void sendUpdatedValues(opencog::CogServer * server); 

public:

    PsiFeelingUpdaterAgent();
    virtual ~PsiFeelingUpdaterAgent();

    virtual const ClassInfo& classinfo() const {
        return info();
    }

    static const ClassInfo& info() {
        static const ClassInfo _ci("OperationalAvatarController::PsiFeelingUpdaterAgent");
        return _ci;
    }

    // Entry of the Agent, CogServer will invoke this function during its cycle
    void run(opencog::CogServer * server);

    // After calling this function, the Agent will invoke its "init" method firstly 
    // in "run" function during its next cycle
    void forceInitNextCycle() {
        this->bInitialized = false;
    }

}; // class

}  // namespace

#endif