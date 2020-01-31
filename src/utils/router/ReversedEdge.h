/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    ReversedEdge.h
/// @author  Michael Behrisch
/// @date    29.01.2020
///
// The ReversedEdge is a wrapper around a ROEdge or a MSEdge used for
// backward search
/****************************************************************************/
#pragma once


// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>


// ===========================================================================
// class definitions
// ===========================================================================
/// @brief the edge type representing backward edges
template<class E, class V>
class ReversedEdge {
public:
    typedef std::vector<std::pair<const ReversedEdge<E, V>*, const ReversedEdge<E, V>*> > ConstEdgePairVector;

    ReversedEdge(const E* orig) : myOriginal(orig) {
    }

    void init() {
        for (const E* edge : myOriginal->getPredecessors()) {
            for (const auto& viaPair : edge->getViaSuccessors()) {
                if (viaPair.first == edge) {
                    myViaSuccessors.push_back(std::make_pair(viaPair.first->getReversedRoutingEdge(), viaPair.second->getReversedRoutingEdge()));
                }
            }
        }
    }

    /** @brief Returns the index (numeric id) of the edge
     * @return The original edge's numerical id
     */
    int getNumericalID() const {
        return myOriginal->getNumericalID();
    }

    /** @brief Returns the id of the edge
     * @return The original edge's id
     */
    const std::string& getID() const {
        return myOriginal->getID();
    }

    /** @brief Returns the length of the edge
     * @return The original edge's length
     */
    double getLength() const {
        return myOriginal->getLength();
    }

    bool isInternal() const {
        return myOriginal->isInternal();
    }

    inline bool prohibits(const V* const vehicle) const {
        return myOriginal->prohibits(vehicle);
    }

    inline bool restricts(const V* const vehicle) const {
        return myOriginal->restricts(vehicle);
    }

    static inline double getTravelTimeStatic(const ReversedEdge<E, V>* const edge, const V* const veh, double time) {
        return edge->myOriginal->getTravelTime(veh, time);
    }

    const ConstEdgePairVector& getViaSuccessors(SUMOVehicleClass vClass = SVC_IGNORING) const {
        //if (vClass == SVC_IGNORING) { // || !MSNet::getInstance()->hasPermissions() || myFunction == EDGEFUNC_CONNECTOR) {
            return myViaSuccessors;
        /*}
#ifdef HAVE_FOX
        FXLock lock(mySuccessorMutex);
#endif
        auto i = myClassesViaSuccessorMap.find(vClass);
        if (i != myClassesViaSuccessorMap.end()) {
            // can use cached value
            return i->second;
        }
        // instantiate vector
        ConstEdgePairVector& result = myClassesViaSuccessorMap[vClass];
        // this vClass is requested for the first time. rebuild all successors
        for (const auto& viaPair : myViaSuccessors) {
            if (viaPair.first->isTazConnector()) {
                result.push_back(viaPair);
            } else {
                const std::vector<MSLane*>* allowed = allowedLanes(*viaPair.first, vClass);
                if (allowed != nullptr && allowed->size() > 0) {
                    result.push_back(viaPair);
                }
            }
        }
        return result;*/
    }

private:
    const E* myOriginal;
    /// @brief The successors available for a given vClass
    mutable std::map<SUMOVehicleClass, ConstEdgePairVector> myClassesViaSuccessorMap;

    mutable ConstEdgePairVector myViaSuccessors;

#ifdef HAVE_FOX
    /// @brief Mutex for accessing successor edges
    mutable FXMutex mySuccessorMutex;
#endif

};


/****************************************************************************/