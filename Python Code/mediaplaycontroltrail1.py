import dbus
bus = dbus.SessionBus()
print(bus.get_unique_name())
bus.request_name('SmartHelmet.test')
# for service in bus.list_names():
#     print(service)
print(bus.get_unique_name())
