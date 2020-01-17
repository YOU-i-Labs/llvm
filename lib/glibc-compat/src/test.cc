/* Copyright 2014 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file. */

#include <endian.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef __GLIBC__
#include <sys/endian.h>
#endif

#include "gtest/gtest.h"

TEST(TestMktemp, mkdtemp_errors) {
  char small[] = "small";
  char missing_template[] = "missing_template";
  char short_template_XXX[] = "short_template_XXX";
  char not_XXXXXX_suffix[] = "not_XXXXXX_suffix";
  ASSERT_EQ((char*)NULL, mkdtemp(small));
  ASSERT_EQ(EINVAL, errno);
  ASSERT_EQ((char*)NULL, mkdtemp(missing_template));
  ASSERT_EQ(EINVAL, errno);
  ASSERT_EQ((char*)NULL, mkdtemp(short_template_XXX));
  ASSERT_EQ(EINVAL, errno);
  ASSERT_EQ((char*)NULL, mkdtemp(not_XXXXXX_suffix));
  ASSERT_EQ(EINVAL, errno);

}

TEST(TestMktemp, mkdtemp) {
  char tempdir[] = "tempfile_XXXXXX";
  ASSERT_NE((char*)NULL, mkdtemp(tempdir));
  // Check that tempname starts with the correct prefix but has been
  // modified from the original.
  ASSERT_EQ(0, strncmp("tempfile_", tempdir, strlen("tempfile_")));
  ASSERT_STRNE("tempfile_XXXXXX", tempdir);

  // Check the directory exists
  struct stat buf;
  ASSERT_EQ(0, stat(tempdir, &buf));
  ASSERT_TRUE(S_ISDIR(buf.st_mode));
  ASSERT_EQ(0, rmdir(tempdir));
}

TEST(TestMktemp, mkstemp) {
  char tempfile[] = "tempfile_XXXXXX";
  int fd = mkstemp(tempfile);
  ASSERT_GT(fd, -1);

  // Check that tempname starts with the correct prefix but has been
  // modified from the original.
  ASSERT_EQ(0, strncmp("tempfile_", tempfile, strlen("tempfile_")));
  ASSERT_STRNE("tempfile_XXXXXX", tempfile);

  ASSERT_EQ(4, write(fd, "test", 4));
  ASSERT_EQ(0, close(fd));
}

TEST(TestEndian, byte_order) {
#ifdef __native_client__
  ASSERT_EQ(LITTLE_ENDIAN, BYTE_ORDER);
  ASSERT_EQ(LITTLE_ENDIAN, BYTE_ORDER);
#endif
}

#ifndef __GLIBC__
TEST(TestEndian, byte_swap) {
  // Test BSD byte-swapping macros
  uint16_t num16 = 0x0102u;
  uint32_t num32 = 0x01020304u;
  ASSERT_EQ(num16, htole16(num16));
  ASSERT_EQ(num32, htole32(num32));
  ASSERT_EQ(num16, letoh16(num16));
  ASSERT_EQ(num32, letoh32(num32));

  // Can't inline these in the ASSERT_EQ statements as they can be
  // 'statement expressions' (on bionic at least) which can't be
  // template parameters apparently.
  int n16 = htons(num16);
  int n32 = htonl(num32);
  int h16 = ntohs(num16);
  int h32 = ntohl(num32);
  ASSERT_EQ(n16, htobe16(num16));
  ASSERT_EQ(n32, htobe32(num32));
  ASSERT_EQ(h16, betoh16(num16));
  ASSERT_EQ(h32, betoh32(num32));
}
#endif

TEST(TestLockf, lockf) {
  // The fcntl() method underlying lockf() is not implemented in NaCl.
  ASSERT_EQ(-1, lockf(1, F_LOCK, 1));
  ASSERT_EQ(ENOSYS, errno);
  ASSERT_EQ(-1, lockf(1, F_TLOCK, 1));
  ASSERT_EQ(ENOSYS, errno);
  ASSERT_EQ(-1, lockf(1, F_ULOCK, 1));
  ASSERT_EQ(ENOSYS, errno);
}

int main(int argc, char** argv) {
  setenv("TERM", "xterm-256color", 0);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
