These are tools designed for working with modern linux system calls including, mincore, fallocate, fadvise, etc.

We designed these primarily to work in high performance environments to determine information about the running kernel, improve system performance, and debug performance problems.

= fincore =

{{{
fincore [options] files...

  --pages=false      Do not print pages
  --summarize        When comparing multiple files, print a summary report
  --only-cached      Only print stats for files that are actually in cache.

root@xxxxxx:/var/lib/mysql/blogindex# fincore --pages=false --summarize --only-cached * 
stats for CLUSTER_LOG_2010_05_21.MYI: file size=93840384 , total pages=22910 , cached pages=1 , cached size=4096, cached perc=0.004365 
stats for CLUSTER_LOG_2010_05_22.MYI: file size=417792 , total pages=102 , cached pages=1 , cached size=4096, cached perc=0.980392 
stats for CLUSTER_LOG_2010_05_23.MYI: file size=826368 , total pages=201 , cached pages=1 , cached size=4096, cached perc=0.497512 
stats for CLUSTER_LOG_2010_05_24.MYI: file size=192512 , total pages=47 , cached pages=1 , cached size=4096, cached perc=2.127660 
stats for CLUSTER_LOG_2010_06_03.MYI: file size=345088 , total pages=84 , cached pages=43 , cached size=176128, cached perc=51.190476 
stats for CLUSTER_LOG_2010_06_04.MYD: file size=1478552 , total pages=360 , cached pages=97 , cached size=397312, cached perc=26.944444 
stats for CLUSTER_LOG_2010_06_04.MYI: file size=205824 , total pages=50 , cached pages=29 , cached size=118784, cached perc=58.000000 
stats for COMMENT_CONTENT_2010_06_03.MYI: file size=100051968 , total pages=24426 , cached pages=10253 , cached size=41996288, cached perc=41.975764 
stats for COMMENT_CONTENT_2010_06_04.MYD: file size=716369644 , total pages=174894 , cached pages=79821 , cached size=326946816, cached perc=45.639645 
stats for COMMENT_CONTENT_2010_06_04.MYI: file size=56832000 , total pages=13875 , cached pages=5365 , cached size=21975040, cached perc=38.666667 
stats for FEED_CONTENT_2010_06_03.MYI: file size=1001518080 , total pages=244511 , cached pages=98975 , cached size=405401600, cached perc=40.478751 
stats for FEED_CONTENT_2010_06_04.MYD: file size=9206385684 , total pages=2247652 , cached pages=1018661 , cached size=4172435456, cached perc=45.321117 
stats for FEED_CONTENT_2010_06_04.MYI: file size=638005248 , total pages=155763 , cached pages=52912 , cached size=216727552, cached perc=33.969556 
stats for FEED_CONTENT_2010_06_04.frm: file size=9840 , total pages=2 , cached pages=3 , cached size=12288, cached perc=150.000000 
stats for PERMALINK_CONTENT_2010_06_03.MYI: file size=1035290624 , total pages=252756 , cached pages=108563 , cached size=444674048, cached perc=42.951700 
stats for PERMALINK_CONTENT_2010_06_04.MYD: file size=55619712720 , total pages=13579031 , cached pages=6590322 , cached size=26993958912, cached perc=48.533080 
stats for PERMALINK_CONTENT_2010_06_04.MYI: file size=659397632 , total pages=160985 , cached pages=54304 , cached size=222429184, cached perc=33.732335 
stats for PERMALINK_CONTENT_2010_06_04.frm: file size=10156 , total pages=2 , cached pages=3 , cached size=12288, cached perc=150.000000 
---
total cached size: 32847278080
}}}

= fadvise =

{{{

SYNTAX: filename mode [offset] [,length]
Where mode can be:

  POSIX_FADV_NORMAL       No further special treatment.  
  POSIX_FADV_RANDOM       Expect random page references.  
  POSIX_FADV_SEQUENTIAL   Expect sequential page references.  
  POSIX_FADV_WILLNEED     Will need these pages.  
  POSIX_FADV_DONTNEED     Dont need these pages.  
  POSIX_FADV_NOREUSE      Data will be accessed once.  

Allows an application to to tell the kernel how it expects to use a file handle,
so that the kernel can choose appropriate read-ahead and caching techniques for
access to the corresponding file. This is similar to the POSIX version of the
madvise system call, but for file access instead of memory access. The
sys_fadvise64() function is obsolete and corresponds to a broken glibc API,
sys_fadvise64_64() is the fixed version. The following are the values for the
advice parameter:

FADV_NORMAL

No special treatment.

FADV_RANDOM

Expect page references in random order.

FADV_SEQUENTIAL

Expect page references in sequential order.

FADV_WILLNEED

Expect access in the near future.

FADV_DONTNEED

Do not expect access in the near future. Subsequent access of pages in this
range will succeed, but will result either in reloading of the memory contents
from the underlying mapped file or zero-fill-in-demand pages for mappings
without an underlying file.

FADV_NOREUSE

Access data only once.
}}}

= fallocate =

{{{
SYNTAX: fallocate file length

fallocate() allows the caller to directly manipulate the allocated disk space
for the file referred to by fd for the byte range starting at offset and
continuing for len bytes.

The mode argument determines the operation to be performed on the given
range. Currently only one flag is supported for mode:

FALLOC_FL_KEEP_SIZE

This flag allocates and initializes to zero the disk space within the range
specified by offset and len. After a successful call, subsequent writes into
this range are guaranteed not to fail because of lack of disk
space. Preallocating zeroed blocks beyond the end of the file is useful for
optimizing append workloads. Preallocating blocks does not change the file size
(as reported by stat(2)) even if it is less than offset+len.

If FALLOC_FL_KEEP_SIZE flag is not specified in mode, the default behavior is
almost same as when this flag is specified. The only difference is that on
success, the file size will be changed if offset + len is greater than the file
size. This default behavior closely resembles the behavior of the
posix_fallocate(3) library function, and is intended as a method of optimally
implementing that function.

Because allocation is done in block size chunks, fallocate() may allocate a
larger range than that which was specified.
}}}
