from xmlUtil import notif_close, notif_open, notification_filter
from ncclient import manager, operations
import settings

class NCConnection():
    """ Base class. Provides a NETCONF connection. """

    def __init__(self):
        self.nc = manager.connect(
            host=settings.HOST,
            port=settings.PORT,
            username=settings.USERNAME,
            password=settings.PASSWORD,
            allow_agent=False,
            look_for_keys=False,
            hostkey_verify=False)
        self.nc.raise_mode = operations.RaiseMode.NONE
        self.nc.create_subscription(filter=notif_open+notification_filter+notif_close)

    def tear_down(self):
        self.nc.close_session()