import unittest
import nctest

class TestSystem(nctest.NCTestCase):
    """ Tests basic NETCONF operations on the device YANG module. """
    def __init__(self, *args, **kwargs):
        super(TestSystem, self).__init__(*args, **kwargs)
        self.ns = {"nc": "urn:ietf:params:xml:ns:netconf:base:1.0", "dev": "http://org/openroadm/device"}

    def test_device_config(self):
        filter_xml = """<nc:filter xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0">
            <open-optical-device xmlns="http://org/openroadm/device">
                 <config/>
            </open-optical-device>
            </nc:filter>"""
        reply = self.nc.get_config(source="running", filter=filter_xml)
        self.check_reply_data(reply)
        #check node-id
        node_id = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:config/dev:node-id", namespaces=self.ns)
        self.assertEqual(len(node_id), 1)
        self.assertTrue(node_id[0].tag.endswith("node-id"))
        #check node-number
        node_number = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:config/dev:node-number", namespaces=self.ns)
        self.assertEqual(len(node_number), 1)
        self.assertTrue(node_number[0].tag.endswith("node-number"))

    def test_device_physical(self):
        filter_xml = """<nc:filter xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0">
            <open-optical-device xmlns="http://org/openroadm/device">
                 <physical/>
            </open-optical-device>
            </nc:filter>"""
        reply = self.nc.get(filter=filter_xml)
        self.check_reply_data(reply)
        #check node-type
        node_type = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:physical/dev:node-type", namespaces=self.ns)
        self.assertEqual(len(node_type), 1)
        self.assertTrue(node_type[0].tag.endswith("node-type"))
        #check vendor
        vendor = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:physical/dev:vendor", namespaces=self.ns)
        self.assertEqual(len(vendor), 1)
        self.assertTrue(vendor[0].tag.endswith("vendor"))
        #check model
        model = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:physical/dev:model", namespaces=self.ns)
        self.assertEqual(len(model), 1)
        self.assertTrue(model[0].tag.endswith("model"))
        #check product-code
        product_code = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:physical/dev:product-code", namespaces=self.ns)
        self.assertEqual(len(product_code), 1)
        self.assertTrue(product_code[0].tag.endswith("product-code"))
        #check serial-id
        serial_id = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:physical/dev:serial-id", namespaces=self.ns)
        self.assertEqual(len(serial_id), 1)
        self.assertTrue(serial_id[0].tag.endswith("serial-id"))
        #check manufactor_date
        manufactor_date = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:physical/dev:manufactor-date", namespaces=self.ns)
        self.assertEqual(len(manufactor_date), 1)
        self.assertTrue(manufactor_date[0].tag.endswith("manufactor-date"))
        #check clei
        clei = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:physical/dev:clei", namespaces=self.ns)
        self.assertEqual(len(clei), 1)
        self.assertTrue(clei[0].tag.endswith("clei"))
        #check hardware-version
        hardware_version = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:physical/dev:hardware-version", namespaces=self.ns)
        self.assertEqual(len(hardware_version), 1)
        self.assertTrue(hardware_version[0].tag.endswith("hardware-version"))
        #check softwareVersion
        softwareVersion = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:physical/dev:softwareVersion", namespaces=self.ns)
        self.assertEqual(len(softwareVersion), 1)
        self.assertTrue(softwareVersion[0].tag.endswith("softwareVersion"))
        '''
        #check module-type
        module_type = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:physical/dev:module-type", namespaces=self.ns)
        self.assertEqual(len(module_type), 1)
        self.assertTrue(module_type[0].tag.endswith("module-type"))
        '''

    def test_device_submodules(self):
        filter_xml = """<nc:filter xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0">
            <open-optical-device xmlns="http://org/openroadm/device">
                <sub-modules/>
            </open-optical-device> 
            </nc:filter>"""
        reply = self.nc.get(filter=filter_xml)
        self.check_reply_data(reply)
        #check sub-modules
        sub_modules = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/*", namespaces=self.ns)
        self.assertNotEqual(len(sub_modules), 0)
        for sub_module in sub_modules:
            self.assertTrue(sub_module.tag.endswith("sub-modules"))
   
    def test_device_state(self):
        filter_xml = """<nc:filter xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0">
            <open-optical-device xmlns="http://org/openroadm/device">
                <state/>
            </open-optical-device> 
            </nc:filter>"""
        reply = self.nc.get(filter=filter_xml)
        self.check_reply_data(reply)
        #check current-datetime
        current_datetime = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:state/dev:current-datetime", namespaces=self.ns)
        self.assertEqual(len(current_datetime), 1)
        self.assertTrue(current_datetime[0].tag.endswith("current-datetime"))
        #check uptime
        uptime = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:state/dev:uptime", namespaces=self.ns)
        self.assertEqual(len(uptime), 1)
        self.assertTrue(uptime[0].tag.endswith("uptime"))
        #check run-time
        run_time = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:state/dev:run-time", namespaces=self.ns)
        self.assertEqual(len(run_time), 1)
        self.assertTrue(run_time[0].tag.endswith("run-time"))
        #check temperature
        temperature = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:state/dev:temperature", namespaces=self.ns)
        self.assertEqual(len(temperature), 1)
        self.assertTrue(temperature[0].tag.endswith("temperature"))
        #check upg-state
        upg_state = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:state/dev:upg-state", namespaces=self.ns)
        self.assertEqual(len(upg_state), 1)
        self.assertTrue(upg_state[0].tag.endswith("upg-state"))
        #check leds
        leds = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:state/dev:leds", namespaces=self.ns)
        self.assertNotEqual(len(leds), 0)
        for led in leds:
            self.assertTrue(led.tag.endswith("leds"))
'''
if __name__ == '__main__':
    unittest.main()
'''

suite = unittest.TestLoader().loadTestsFromTestCase(TestSystem)
unittest.TextTestRunner(verbosity=2).run(suite)
