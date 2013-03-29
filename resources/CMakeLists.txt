FILE (GLOB resourceFiles *.png *.wav *.ogg)

#FILE (COPY ${resourceFiles} DESTINATION .) 

FOREACH (resourceFile ${resourceFiles})
	execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${resourceFile} ${CMAKE_CURRENT_BINARY_DIR})
ENDFOREACH(resourceFile)

ADD_SUBDIRECTORY (Fonts) 
