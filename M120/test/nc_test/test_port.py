import unittest
import nctest

class TestPort(nctest.NCTestCase):
    """ Tests basic NETCONF operations on port. """
    def __init__(self, *args, **kwargs):
        super(TestPort, self).__init__(*args, **kwargs)
        self.ns = {"nc": "urn:ietf:params:xml:ns:netconf:base:1.0", "dev": "http://org/openroadm/device"}

    def test_ports(self):
        filter_xml = """<nc:filter xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0">
            <open-optical-device xmlns="http://org/openroadm/device">
                <ports>
                    <port/>
                </ports>
            </open-optical-device>
            </nc:filter>"""
        reply = self.nc.get(filter=filter_xml)
        self.check_reply_data(reply)
        #check node-id
        ports = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/*", namespaces=self.ns)
        self.assertNotEqual(len(ports), 0)
        for port in ports:
            self.assertTrue(port.tag.endswith("ports"))
        #check port-name
        port_names = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:ports/dev:port/dev:port-name", namespaces=self.ns)
        self.assertNotEqual(len(port_names), 0)
        #check port-direction
        port_directions = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:ports/dev:port/dev:port-direction", namespaces=self.ns)
        self.assertNotEqual(len(port_directions), 0)
        #check port-type
        port_types = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:ports/dev:port/dev:port-type", namespaces=self.ns)
        self.assertNotEqual(len(port_types), 0)
        #check port-qual-type
        port_qual_types = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:ports/dev:port/dev:port-qual-type", namespaces=self.ns)
        self.assertNotEqual(len(port_qual_types), 0)
        '''#check port-target-power
        port_target_powers= reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:ports/dev:port/dev:port-target-power", namespaces=self.ns)
        self.assertNotEqual(len(port_target_powers), 0)'''
        #check port-current-power
        port_current_powers= reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:ports/dev:port/dev:port-current-power", namespaces=self.ns)
        self.assertNotEqual(len(port_current_powers), 0)
        #check edfa-id
        edfa_ids= reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:ports/dev:port/dev:edfa-id", namespaces=self.ns)
        self.assertNotEqual(len(edfa_ids), 0)
        #check osc-id
        osc_ids= reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:ports/dev:port/dev:osc-id", namespaces=self.ns)
        self.assertNotEqual(len(osc_ids), 0)
        #check port-property
        port_properties= reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:ports/dev:port/dev:port-property", namespaces=self.ns)
        self.assertNotEqual(len(port_properties), 0)
        #check used-wavelengths
        used_wavelengths= reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:ports/dev:port/dev:used-wavelengths", namespaces=self.ns)
        self.assertNotEqual(len(used_wavelengths), 0)

'''
if __name__ == '__main__':
    unittest.main()
'''
suite = unittest.TestLoader().loadTestsFromTestCase(TestPort)
unittest.TextTestRunner(verbosity=2).run(suite)

