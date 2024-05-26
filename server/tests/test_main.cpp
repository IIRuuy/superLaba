#include "googletest/googletest/include/gtest/gtest.h"
#include "../src/Repository/UserRepository.h"

TEST(UserRepositoryTest, FindRootUser) {
  std::set<RoleEntity> roles;
  roles.insert(RoleEntity::ADMIN);
  UserEntity user(0, "root", "root", "123", 0, roles);

  UserRepository userRepository;
  UserEntity assertedUser = userRepository.getByUsername("root");
  ASSERT_TRUE(assertedUser==user);
}

TEST(UserRepositoryTest, CreateUser) {
  UserRepository userRepository;
  UserEntity onCreateUser("ruuy", "123", "daniil", 15);
  UserEntity assertedUser = userRepository.addUser(onCreateUser);
  
  std::set<RoleEntity> roles;
  roles.insert(RoleEntity::USER);
  UserEntity user(1, "ruuy", "daniil", "123", 15, roles);

  ASSERT_TRUE(assertedUser==user);
}

TEST(UserRepositoryTest, FindByUsername) {
  UserRepository userRepository;
  ASSERT_TRUE(userRepository.existByUsername("root2"));
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}