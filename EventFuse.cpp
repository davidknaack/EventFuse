/*
 * EventFuse.cpp - A simple fuse-based event callback library
 * v0.1 - 2009-June-16
 * v0.2 - 2009-June-16 Added fsBlocked state to handle callback reentrance.
 * v0.3 - 2012-April-27
 *   - Minor update to burn() to out or range issue in repeatCount.
 *     This caused problems with INF_REPEAT fuses.
* Derived from:
 * Timer.cpp - A Real-time Timer Library for Arduino & Wiring
 * Copyright (c) 2009 Daniel Bradberry.  All right reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "EventFuse.h"

// An instance of the EventFuse class
EventFuse eventFuse = EventFuse();

EventFuse::EventFuse() {
	for(unsigned char i=0; i<MAX_FUSES; i++) {
		// init fuses to unallocated state
		fuses[i].fuseState = fsUnallocated;
	}
}

void EventFuse::resetFuse(FuseID fuse, int userData, int fuseLen, unsigned int repeatCount, eventFuseCallback_t fuseCallback)
{
	fuses[fuse].fuseState = fsEnabled;
	fuses[fuse].fuseInit = fuseLen;
	fuses[fuse].fuseLen = fuseLen;
	fuses[fuse].repeatCount = repeatCount;
	fuses[fuse].userData = userData;
	fuses[fuse].callback = fuseCallback;
}

void EventFuse::resetFuse(FuseID fuse, int fuseLen, unsigned int repeatCount, eventFuseCallback_t fuseCallback)
{
	resetFuse( fuse, 0, fuseLen, repeatCount, fuseCallback );
}

FuseID EventFuse::newFuse(int fuseLen, unsigned int repeatCount, eventFuseCallback_t fuseCallback)
{
  return newFuse( 0, fuseLen, repeatCount, fuseCallback );
}

FuseID EventFuse::newFuse(int userData, int fuseLen, unsigned int repeatCount, eventFuseCallback_t fuseCallback)
{
	// Repeat count must be at least one or INF_REPEAT
	if (repeatCount<1)
		return NULL_FUSE;

	// search for the first unallocated fuse
	unsigned char i;
	for(i=0; i<MAX_FUSES; i++) {
		if(fuses[i].fuseState == fsUnallocated)
			break;
	}
	
	// no fuses available
	if(i == MAX_FUSES) 
		return NULL_FUSE;
	
	resetFuse( i, userData, fuseLen, repeatCount, fuseCallback );
	
	return i;
}

void EventFuse::burn(int len) {
	for(unsigned char i=0; i<MAX_FUSES; i++) {
		if(fuses[i].fuseState == fsEnabled) {
			// burn the fuse the specified length
			fuses[i].fuseLen -= len;

			// if the fuse has burned down to (or past) zero
			if(fuses[i].fuseLen <= 0) {
				// Disable the fuse during the callback. This prevents
				// reentering the callback if burn() is called again
				// such as by an interrupt.
				FuseState tState = fuses[i].fuseState;
				fuses[i].fuseState = fsBlocked;

				// Exec the callback and pass the relevant fuse.
				// The user may check the fuseLen if necessary
				// to compensate for overshoot.
				// The user may also reset the fuse length for
				// the next burn, adjust the repeat count, etc.
				int tLen = fuses[i].fuseLen;			
				fuses[i].callback(i,fuses[i].userData);
				
				// restore the fuse state if it hasn't been updated
				if (fuses[i].fuseState == fsBlocked)
				  fuses[i].fuseState = tState;
				
				// if fuseLen wasn't changed in the callback
				// reset it to fuseInit. If it was changed then
				// that value will be the new fuse length, overriding
				// the value specified in fuseInit.
				if(tLen == fuses[i].fuseLen)
					fuses[i].fuseLen = fuses[i].fuseInit;

				if((fuses[i].repeatCount != INF_REPEAT))
					fuses[i].repeatCount--;

				if(fuses[i].repeatCount == 0)
					fuses[i].fuseState = fsUnallocated;			
			}
		}
	}
}
