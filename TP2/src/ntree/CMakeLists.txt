cmake_minimum_required(VERSION 3.5)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

project(List C)
# add_executable(ntree ntree.c ntree.h)
add_library(ntree SHARED ntree.c ntree.h)

install(
	TARGETS ntree
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
)

install(
	FILES ntree.h
	DESTINATION include
)

# Ajout d'un fichier de configuration de type pkgconfig. Copie le 1er argument vers le 2ème. @ONLY = restreint le remplacement de variable dans ntree.pc.in
# à celles qui ont le format @<var>@ pour éviter les conflits avec la syntaxe CMake ${<var>}.
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/ntree.pc.in
	${CMAKE_CURRENT_BINARY_DIR}/ntree.pc
	@ONLY
)
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/ntree.pc
	DESTINATION share/pkgconfig
	COMPONENT "PkgConfig"
)

#  Ajout d'un fichier de configuration de type cmake
include(CMakePackageConfigHelpers)
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/NTreeConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/NTreeConfig.cmake
	INSTALL_DESTINATION cmake
)
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/NTreeConfig.cmake
	DESTINATION cmake
)

# Ajout d'un exécutable dépendant également de tree.h
add_executable(test-ntree test-ntree.c ntree.h)
# Précision de l'ordre de construction: le programme de test doit se faire
# après la librairie
add_dependencies(test-ntree ntree)
# Aggrégation du programme de test avec la librairie tree
target_link_libraries(test-ntree ntree)

# Activation des tests
enable_testing()
# Ajout d'un test
add_test(test-ntree ./test-ntree)
