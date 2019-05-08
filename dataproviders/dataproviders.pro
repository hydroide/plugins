TEMPLATE = subdirs

qtHaveModule(sql) {
    SUBDIRS += sqlitedataprovider
}
