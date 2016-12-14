/ hanoi.c
//
// The Towers of Hanoi as an Extensible Firmware Interface application.
// Copyright (c) 2006 Amit Singh. All Rights Reserved.
// <www.osxbook.com>
//

#include <efi.h>      // pulls in public EFI header files
#include <efilib.h>   // EFI library header

void dohanoi(int n, int from, int to, int using);
INTN populate_argv(CHAR16 *buf, UINTN len, CHAR16 **argv);

enum {
	FROMTOWER = 1,  // move disks "from" this tower ID
	USINGTOWER = 2,  // move disks "using" this tower ID
	TOTOWER = 3,  // move disks "to" this tower ID
	NDISKS_MIN = 1,  // lower bound on the number of disks
	NDISKS_MAX = 10, // upper bound on the number of disks
	MAX_ARGS = 32, // upper bound on the number of command-line arguments
};

// our application's entry function
EFI_STATUS
efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *systab)
{
	UINTN             ndisks;
	INTN              argc = 0;
	CHAR16           *argv[MAX_ARGS];
	CHAR16           *rawargs = NULL;
	EFI_LOADED_IMAGE *info;
	EFI_STATUS        status;

	// Initialize the EFI library.
	InitializeLib(image, systab);

	// After the EFI library is initialized, we can access the EFI system
	// table, the EFI boot services table, and the EFI runtime services
	// table through 'ST', 'BS', and 'RT', respectively.

	// Prepare to gather command-line arguments. First, retrieve image info.
	status = BS->HandleProtocol(image, &LoadedImageProtocol, (VOID *)&info);
	if (EFI_ERROR(status)) {
		Print(L"%r = BS->HandleProtocol(..., &LoadedImageProtocol, ...)\n",
			status);
		return status;
	}

	// Allocate memory for processing command-line arguments.
	status = BS->AllocatePool(EfiLoaderData,
		info->LoadOptionsSize + sizeof(CHAR16),
		(VOID *)&rawargs);
	if (status != EFI_SUCCESS) {
		Print(L"%r = BS->AllocatePool(...)\n", status);
		return status;
	}

	// Populate C-style argc and argv.
	CopyMem(rawargs, info->LoadOptions, info->LoadOptionsSize);
	argc = populate_argv(rawargs, info->LoadOptionsSize, argv);

	if (argc != 2) {
		Print(L"The Towers of Hanoi <www.osxbook.com>\n");
		Print(L"usage: %s N\n", argv[0]);
		status = EFI_INVALID_PARAMETER;
		goto out;
	}

	ndisks = Atoi((CHAR16 *)argv[1]);

	if ((ndisks < NDISKS_MIN) || (ndisks > NDISKS_MAX)) {
		Print(L"%s: illegal value for number of disks\n", argv[0]);
		status = EFI_INVALID_PARAMETER;
		goto out;
	}

	dohanoi(ndisks, FROMTOWER, TOTOWER, USINGTOWER);

out:
	(void)BS->FreePool((VOID *)rawargs);

	return status;
}

void
dohanoi(int n, int from, int to, int using)
{
	if (n > 0) {
		dohanoi(n - 1, from, using, to);
		Print(L"move %d --> %d\n", from, to);
		dohanoi(n - 1, using, to, from);
	}
}

// the following function is stolen/adapted from elilo's argify()
//
// Copyright (C) 2001-2003 Hewlett-Packard Co.
//     Contributed by Stephane Eranian <eranian@hpl.hp.com>
//  Copyright (C) 2001 Silicon Graphics, Inc.
//      Contributed by Brent Casavant <bcasavan@sgi.com>

INTN
populate_argv(CHAR16 *buf, UINTN len, CHAR16 **argv)
{
	UINTN   i = 0;
	UINTN   j = 0;
	CHAR16 *p = buf;
#define CHAR_SPACE L' '

	if (buf == 0) {
		argv[0] = NULL;
		return 0;
	}

	// len is the number of bytes (and not the number of CHAR16's)
	len = len >> 1;

	// Here we use CHAR_NULL as the terminator rather than the length because
	// the EFI shell returns rather bogus values for it. Apparently, we are
	// guaranteed to find '\0' in the buffer where the real input arguments
	// stop, so we use it instead.
	// 
	for (;;) {
		while (buf[i] == CHAR_SPACE && buf[i] != CHAR_NULL && i < len)
			i++;

		if (buf[i] == CHAR_NULL || i == len)
			goto end;

		p = buf + i;
		i++;

		while (buf[i] != CHAR_SPACE && buf[i] != CHAR_NULL && i < len)
			i++;

		argv[j++] = p;

		if (buf[i] == CHAR_NULL)
			goto end;

		buf[i] = CHAR_NULL;

		if (i == len)
			goto end;

		i++;

		if (j == (MAX_ARGS - 1)) {
			Print(L"%s: too many arguments (%d), truncating", argv[0], j);
			goto end;
		}
	}

end:
	argv[j] = NULL;
	return j;
}
