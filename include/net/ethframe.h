#ifndef __REXOS__NET__ETHFRAME_H
#define __REXOS__NET__ETHFRAME_H
#include <common/types.h>
#include <drivers/amd_am79c973.h>
#include <heap.h>

namespace rexos {
    namespace net {
        struct EthernetFrameHeader {
            common::uint64_t dstMAC_BE : 48;
            common::uint64_t srcMAC_BE : 48;
            common::uint16_t etherType_BE;
        } __attribute__ ((packed));
        typedef common::uint32_t EthernetFrameFooter;

        class EthernetFrameProvider;
        // Making an handler class from which ARP, ICMP, UDP, TCP and so on
        // can be derived 
        class EthernetFrameHandler {
            protected:
                EthernetFrameProvider* backend;
                common::uint16_t etherType_BE;
            public:
                EthernetFrameHandler(EthernetFrameProvider* backend, common::uint16_t etherType);
                ~EthernetFrameHandler();

                virtual bool OnEthernetFrameReceived(common::uint8_t* etherFramePayload,
                                                    common::uint32_t size);
                // Calls the send function of backend (EthernetFrameProvider)
                void Send(common::uint64_t dstMAC_BE, common::uint8_t* etherFramePayload,
                                                    common::uint32_t size);
                common::uint32_t GetIPAddress();
        };


        class EthernetFrameProvider : public drivers::RawDataHandler {
            friend class EthernetFrameHandler;
            protected:
                // EthernetFrameProvider will have an array of pointers to the
                // handler
                EthernetFrameHandler* handlers[65535];
            public:
                EthernetFrameProvider(drivers::amd_am79c973* backend);
                ~EthernetFrameProvider();
                bool OnRawDataReceived(common::uint8_t* rawData, common::uint32_t size);
                void Send(common::uint64_t dstMAC_BE, common::uint16_t etherType_BE, 
                        common::uint8_t* buffer, common::uint32_t size);
                // EthernetFrameProvider will get the IP address from the net
                // driver's initBlock
                common::uint32_t GetIPAddress();
                common::uint64_t GetMACAddress();
        };
    }
}





#endif