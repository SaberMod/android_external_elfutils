/* Return block represented by attribute.
   Copyright (C) 2004, 2005 Red Hat, Inc.
   This file is part of Red Hat elfutils.
   Written by Ulrich Drepper <drepper@redhat.com>, 2004.

   Red Hat elfutils is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by the
   Free Software Foundation; version 2 of the License.

   Red Hat elfutils is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with Red Hat elfutils; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301 USA.

   In addition, as a special exception, Red Hat, Inc. gives You the
   additional right to link the code of Red Hat elfutils with code licensed
   under any Open Source Initiative certified open source license
   (http://www.opensource.org/licenses/index.php) which requires the
   distribution of source code with any binary distribution and to
   distribute linked combinations of the two.  Non-GPL Code permitted under
   this exception must only link to the code of Red Hat elfutils through
   those well defined interfaces identified in the file named EXCEPTION
   found in the source code files (the "Approved Interfaces").  The files
   of Non-GPL Code may instantiate templates or use macros or inline
   functions from the Approved Interfaces without causing the resulting
   work to be covered by the GNU General Public License.  Only Red Hat,
   Inc. may make changes or additions to the list of Approved Interfaces.
   Red Hat's grant of this exception is conditioned upon your not adding
   any new exceptions.  If you wish to add a new Approved Interface or
   exception, please contact Red Hat.  You must obey the GNU General Public
   License in all respects for all of the Red Hat elfutils code and other
   code used in conjunction with Red Hat elfutils except the Non-GPL Code
   covered by this exception.  If you modify this file, you may extend this
   exception to your version of the file, but you are not obligated to do
   so.  If you do not wish to provide this exception without modification,
   you must delete this exception statement from your version and license
   this file solely under the GPL without exception.

   Red Hat elfutils is an included package of the Open Invention Network.
   An included package of the Open Invention Network is a package for which
   Open Invention Network licensees cross-license their patents.  No patent
   license is granted, either expressly or impliedly, by designation as an
   included package.  Should you wish to participate in the Open Invention
   Network licensing program, please visit www.openinventionnetwork.com
   <http://www.openinventionnetwork.com>.  */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <dwarf.h>
#include "libdwP.h"


int
dwarf_formblock (attr, return_block)
     Dwarf_Attribute *attr;
     Dwarf_Block *return_block;
{
  if (attr == NULL)
    return -1;

  const unsigned char *datap;

  switch (attr->form)
    {
    case DW_FORM_block1:
      return_block->length = *(uint8_t *) attr->valp;
      return_block->data = attr->valp + 1;
      break;

    case DW_FORM_block2:
      return_block->length = read_2ubyte_unaligned (attr->cu->dbg, attr->valp);
      return_block->data = attr->valp + 2;
      break;

    case DW_FORM_block4:
      return_block->length = read_4ubyte_unaligned (attr->cu->dbg, attr->valp);
      return_block->data = attr->valp + 4;
      break;

    case DW_FORM_block:
      datap = attr->valp;
      get_uleb128 (return_block->length, datap);
      return_block->data = (unsigned char *) datap;
      break;

    default:
      __libdw_seterrno (DWARF_E_NO_BLOCK);
      return -1;
    }

  if (return_block->data + return_block->length
      > ((unsigned char *) attr->cu->dbg->sectiondata[IDX_debug_info]->d_buf
	 + attr->cu->dbg->sectiondata[IDX_debug_info]->d_size))
    {
      /* Block does not fit.  */
      __libdw_seterrno (DWARF_E_INVALID_DWARF);
      return -1;
    }

  return 0;
}
INTDEF(dwarf_formblock)
