/* 
   Copyright (C) 2013 Ronnie Sahlberg <ronniesahlberg@gmail.com>
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>

#include <CUnit/CUnit.h>

#include "iscsi.h"
#include "scsi-lowlevel.h"
#include "iscsi-test-cu.h"

void
test_write10_0blocks(void)
{
	int ret;

	CHECK_FOR_DATALOSS;

	logging(LOG_VERBOSE, LOG_BLANK_LINE);
	logging(LOG_VERBOSE, "Test WRITE10 0-blocks at LBA==0");
	ret = write10(iscsic, tgt_lun, 0, 0, block_size,
		     0, 0, 0, 0, 0, NULL);
	if (ret == -2) {
		logging(LOG_NORMAL, "[SKIPPED] WRITE10 is not implemented.");
		CU_PASS("WRITE10 is not implemented.");
		return;
	}	
	CU_ASSERT_EQUAL(ret, 0);

	if (num_blocks >= 0x80000000) {
		CU_PASS("LUN is too big for read-beyond-eol tests with WRITE10. Skipping test.\n");
		return;
	}

	logging(LOG_VERBOSE, "Test WRITE10 0-blocks one block past end-of-LUN");
	ret = write10_lbaoutofrange(iscsic, tgt_lun, num_blocks + 1, 0,
				    block_size, 0, 0, 0, 0, 0, NULL);
	CU_ASSERT_EQUAL(ret, 0);


	logging(LOG_VERBOSE, "Test WRITE10 0-blocks at LBA==2^31");
	ret = write10_lbaoutofrange(iscsic, tgt_lun, 0x80000000, 0,
				    block_size, 0, 0, 0, 0, 0, NULL);
	CU_ASSERT_EQUAL(ret, 0);


	logging(LOG_VERBOSE, "Test WRITE10 0-blocks at LBA==-1");
	ret = write10_lbaoutofrange(iscsic, tgt_lun, -1, 0, block_size,
				    0, 0, 0, 0, 0, NULL);
	CU_ASSERT_EQUAL(ret, 0);
}
