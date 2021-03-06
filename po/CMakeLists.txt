# for i18n

SET(POFILES ar.po bg.po ca.po cs.po da.po de_DE.po el_GR.po en.po en_US.po es_ES.po fi.po fr_FR.po
he.po hi.po hr.po hu.po id.po it_IT.po ja_JP.po ko_KR.po lt.po lv.po ms.po nl_NL.po no.po pl.po
pt_PT.po ro.po ru_RU.po sk.po sl.po sr.po sv.po th.po tr_TR.po uk.po vi.po zh_CN.po zh_HK.po zh_TW.po)

SET(MSGFMT "/usr/bin/msgfmt")

FOREACH(pofile ${POFILES})
	SET(pofile ${CMAKE_CURRENT_SOURCE_DIR}/${pofile})
	MESSAGE("PO: ${pofile}")
	GET_FILENAME_COMPONENT(absPofile ${pofile} ABSOLUTE)
	GET_FILENAME_COMPONENT(lang ${absPofile} NAME_WE)
	SET(moFile ${CMAKE_CURRENT_BINARY_DIR}/${lang}.mo)
	ADD_CUSTOM_COMMAND(
			OUTPUT ${moFile}
			COMMAND ${MSGFMT} -o ${moFile} ${absPofile}
			DEPENDS ${absPofile}
	)
	INSTALL(FILES ${moFile}
			DESTINATION /opt/ug/res/locale/${lang}/LC_MESSAGES RENAME ${PROJECT_NAME}.mo)
	SET(moFiles ${moFiles} ${moFile})
ENDFOREACH(pofile)

MESSAGE(".mo files: ${moFiles}")
ADD_CUSTOM_TARGET(po ALL DEPENDS ${moFiles})
