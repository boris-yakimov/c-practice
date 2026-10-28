
// Devices are represented by entries in the /dev directory. Each device has a
// corresponding device driver, which implements a standard set of operations,
// including those corresponding to the open(), read(), write(), and close()
// system calls. A device may be real, meaning that there is a corresponding
// hardware device, or virtual, meaning that no hardware device exists, but the
// kernel nevertheless provides a device driver that implements an API that is
// the same as a real device.
//
// A hard disk is divided into one or more partitions, each of which may contain
// a file system. A file system is an organized collection of regular files and
// directories. Linux implements a wide variety of file systems, including the
// traditional ext2 file system. The ext2 file system is conceptually similar to
// early UNIX file systems, consisting of a boot block, a superblock, an i-node
// table, and a data area containing file data blocks. Each file has an entry in
// the file system’s i-node table. This entry contains various information about
// the file, including its type, size, link count, ownership, permissions,
// timestamps, and pointers to the file’s data blocks.
//
// Linux provides a range of journaling file systems, including Reiserfs, ext3,
// ext4, XFS, JFS, and Btrfs. A journaling file system records metadata updates
// (and optionally on some file systems, data updates) to a log file before the
// actual file updates are performed. This means that in the event of a system
// crash, the log file can be replayed to quickly restore the file system to a
// consistent state. The key benefit of journaling file systems is that they
// avoid the lengthy file-system consistency checks required by conventional
// UNIX file systems after a system crash.
