FILE (GLOB fontFiles *.ttf *.TXT)

#FILE (COPY ${fontFiles} DESTINATION .) 

FOREACH (fontFile ${fontFiles})
	execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${fontFile} ${CMAKE_CURRENT_BINARY_DIR})
ENDFOREACH(fontFile)


