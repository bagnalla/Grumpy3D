include_directories(${Grumpy3D_SOURCE_DIR}/puddi/include ${Grumpy3D_SOURCE_DIR}/include)

add_executable(Grumpy3D
	main.cc AST.cc GrumpyConfig.cc Lexer.cc RTL.cc
	RTLCompiler.cc SourceCode.cc SyntaxParser.cc
	Token.cc TokenQueue.cc Tree.cc TypeChecker.cc)

target_link_libraries(Grumpy3D puddi gomp GLEW GL SDL2 png jpeg z SDL2_image assimp)
