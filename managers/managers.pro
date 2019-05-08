TEMPLATE = subdirs

SUBDIRS += \
    projectmanager

qtHaveModule(sql) {
    SUBDIRS += datamanager
}
