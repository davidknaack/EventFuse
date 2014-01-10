/*
 * EventFuse.h - A simple fuse-based event callback library
 * v0.4 - 2013-Dec-20
 *   - Converted to namespace.
 *   - added burn() to burn by one.
 *   - changed callback user data to int&
 *   - added cancel(FuseID) 
 * v0.3 - 2012-April-27
 *   - Minor update to burn(int) to out or range issue in repeatCount.
 *     This caused problems with INF_REPEAT fuses.
 * v0.2 - 2009-July-20
 *   - Reworked interface and renamed some
 *     types to improve usability.
 * v0.1 - 2009-June-16
 * Derived from:
 * Timer.h - A Real-time Timer Library for Arduino & Wiring
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

#ifndef EventFuseNs_h
#define EventFuseNs_h

#define INF_REPEAT -1
typedef unsigned char FuseID;  // a fuse handle

namespace EventFuse {

	#define MAX_FUSES 10
	#define NULL_FUSE -1
	
	typedef void (*eventFuseCallback_t)(FuseID, int&);

	enum FuseState {
		fsUnallocated,   // fuse not in use
		fsEnabled,       // fuse is running
		fsDisabled,      // fuse is not running
		fsBlocked,		 // fuse is blocked by callback
	};

	struct eventFuse_t {
		FuseState fuseState;			// current state of the fuse
		int fuseLen;					// current length of the fuse
		int fuseInit;					// internal, starting fuse length
		unsigned int repeatCount;		// number of repeats, INF_REPEAT for infinite
		int userData;					// available for user-defined data
		eventFuseCallback_t callback;	// callback to execute when fuse runs out
	
		void cancel(){ fuseState = fsUnallocated; }
	};

	void init();

	FuseID newFuse(int userData, int fuseLen, unsigned int repeatCount, eventFuseCallback_t fuseCallback);
	FuseID newFuse(int fuseLen, unsigned int repeatCount, eventFuseCallback_t fuseCallback);
	
	void resetFuse(FuseID fuse, int userData, int fuseLen, unsigned int repeatCount, eventFuseCallback_t fuseCallback);
	void resetFuse(FuseID fuse, int fuseLen, unsigned int repeatCount, eventFuseCallback_t fuseCallback);

	void burn(int len);
	void burn();
	
	extern eventFuse_t fuses[MAX_FUSES];
};

#endif