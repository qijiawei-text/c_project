import unittest
import nctest

class TestConnection(nctest.NCTestCase):
    """ Tests basic NETCONF operations on connection. """
    def __init__(self, *args, **kwargs):
        super(TestConnection, self).__init__(*args, **kwargs)
        self.ns = {"nc": "urn:ietf:params:xml:ns:netconf:base:1.0", "dev": "http://org/openroadm/device"}

    #get - should be empty
    def test_get_connection(self):
        filter_xml = """<nc:filter xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0">
            <open-optical-device xmlns="http://org/openroadm/device">
                 <connections/>
            </open-optical-device>
            </nc:filter>"""
        reply = self.nc.get_config(source='running', filter=filter_xml)
        self.check_reply_data(reply)
        connections = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:connections", namespaces=self.ns)
        self.assertEqual(len(connections), 0)
 
    def test_edit_connection(self):
        config_xml = """<nc:config xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0">
            <open-optical-device xmlns="http://org/openroadm/device">
                <connections nc:operation="{}">
                    <connection-id>{}</connection-id>
                    <source>
                        <port-id>3</port-id>
                        <wavelength-id>{}</wavelength-id>
                    </source>
                    <destination>
                        <port-id>2</port-id>
                    </destination>
                    <attenuation>{}</attenuation>
                    <target-output-power>20</target-output-power>
                </connections>
            </open-optical-device>
        </nc:config>"""

        delete_xml = """<nc:config xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0">
            <open-optical-device xmlns="http://org/openroadm/device">
                <connections nc:operation="{}">
                    <connection-id>{}</connection-id>
                </connections>
            </open-optical-device>
        </nc:config>"""

        filter_xml = """<nc:filter xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0">
            <open-optical-device xmlns="http://org/openroadm/device">
                 <connections/>
            </open-optical-device>
            </nc:filter>"""

        # create
        reply = self.nc.edit_config(target='running', config=config_xml.format("create", 1, 1, 10))
        self.check_reply_ok(reply)
        reply = self.nc.get_config(source='running', filter=filter_xml)
        self.check_reply_ok(reply)
        connections = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:connections", namespaces=self.ns)
        self.assertEqual(len(connections), 1);
        # merge
        reply = self.nc.edit_config(target='running', config=config_xml.format("merge", 2, 2, 10))
        self.check_reply_ok(reply)
        reply = self.nc.get_config(source='running', filter=filter_xml)
        self.check_reply_ok(reply)
        connections = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:connections", namespaces=self.ns)
        self.assertEqual(len(connections), 2);
        # replace
        reply = self.nc.edit_config(target='running', config=config_xml.format("replace", 2, 2, 15))
        self.check_reply_ok(reply)
        reply = self.nc.get_config(source='running', filter=filter_xml)
        self.check_reply_ok(reply)        
        attn = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:connections[dev:connection-id='2']/dev:attenuation", namespaces=self.ns)
        self.assertEqual(attn[0].text, "15.0")
        # delete
        reply = self.nc.edit_config(target='running', config=delete_xml.format("delete", 1))
        self.check_reply_ok(reply)
        reply = self.nc.edit_config(target='running', config=delete_xml.format("delete", 2))
        self.check_reply_ok(reply)
        reply = self.nc.get_config(source='running', filter=filter_xml)
        self.check_reply_ok(reply)
        connections = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:connections", namespaces=self.ns)
        self.assertEqual(len(connections), 0);

'''
if __name__ == '__main__':
    unittest.main()
'''

suite = unittest.TestLoader().loadTestsFromTestCase(TestConnection)
unittest.TextTestRunner(verbosity=2).run(suite)

