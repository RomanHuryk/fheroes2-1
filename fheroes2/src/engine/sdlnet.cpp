/***************************************************************************
 *   Copyright (C) 2009 by Andrey Afletdinov                               *
 *   afletdinov@mail.dc.baikal.ru                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifdef WITH_NET

#include <algorithm>
#include <cstring>
#include "sdlnet.h"

#define BUFSIZE 16

namespace Network
{
    static u16 proto = 0xFF01;
};

void Network::SetProtocolVersion(u16 v)
{
    proto = v;
}

Network::Message::Message() : type(0), data(NULL), itd1(NULL), itd2(NULL), dtsz(BUFSIZE)
{
    data = new char [dtsz + 1];

    itd1 = data;
    itd2 = itd1;
}

Network::Message::Message(u16 id) : type(id), data(NULL), itd1(NULL), itd2(NULL), dtsz(BUFSIZE)
{
    data = new char [dtsz + 1];

    itd1 = data;
    itd2 = itd1;
}

Network::Message::Message(const Message & msg) : type(msg.type), data(NULL), itd1(NULL), itd2(NULL), dtsz(msg.dtsz)
{
    data = new char [dtsz + 1];

    std::memcpy(data, msg.data, dtsz);
    itd1 = &data[msg.itd1 - msg.data];
    itd2 = &data[msg.itd2 - msg.data];
}

Network::Message & Network::Message::operator= (const Message & msg)
{
    type = msg.type;
    dtsz = msg.dtsz;
    data = new char [dtsz + 1];

    std::memcpy(data, msg.data, dtsz);
    itd1 = &data[msg.itd1 - msg.data];
    itd2 = &data[msg.itd2 - msg.data];

    return *this;
}

Network::Message::~Message()
{
    if(data) delete [] data;
}

void Network::Message::Resize(size_t lack)
{
    const size_t newsize = lack > dtsz ? dtsz + lack + 1 : 2 * dtsz + 1;
    char* dat2 = new char [newsize];
    std::memcpy(dat2, data, dtsz);
    itd1 = &dat2[itd1 - data];
    itd2 = &dat2[itd2 - data];
    dtsz = newsize - 1;
    delete [] data;
    data = dat2;
}

size_t Network::Message::Size(void) const
{
    return itd2 - data;
}

u16 Network::Message::GetID(void) const
{
    return type;
}

void Network::Message::SetID(u16 id)
{
    type = id;
}

void Network::Message::Reset(void)
{
    type = 0;

    itd1 = data;
    itd2 = itd1;
}

bool Network::Message::Recv(const Socket & csd, bool debug)
{
    u16 head;
    type = 0;

    if(csd.Recv(reinterpret_cast<char *>(&head), sizeof(head)))
    {
	SwapBE16(head);

	// check id
	if((0xFF00 & proto) != (0xFF00 & head))
	{
	    if(debug) std::cerr << "Network::Message::Recv: unknown packet id: 0x" << std::hex << head << std::endl;
	    return true;
	}

	// check ver
	if((0x00FF & proto) > (0x00FF & head))
	{
	    if(debug) std::cerr << "Network::Message::Recv: obsolete protocol proto: 0x" << std::hex << (0x00FF & head) << std::endl;
	    return true;
	}

	u32 size;

	if(csd.Recv(reinterpret_cast<char *>(&type), sizeof(type)) &&
	   csd.Recv(reinterpret_cast<char *>(&size), sizeof(size)))
	{
	    type = SDL_SwapBE16(type);
	    size = SDL_SwapBE32(size);

	    if(size > dtsz)
	    {
		delete [] data;
		data = new char [size + 1];
		dtsz = size;
	    }

	    itd1 = data;
	    itd2 = itd1 + size;

	    return size ? csd.Recv(data, size) : true;
	}
    }
    return false;
}

bool Network::Message::Send(const Socket & csd) const
{
    // raw data
    if(0 == type)
	return Size() ? csd.Send(reinterpret_cast<const char *>(data), Size()) : false;

    u16 head = proto;
    u16 sign = type;
    u32 size = Size();

    SwapBE16(head);
    SwapBE16(sign);
    SwapBE32(size);

    return csd.Send(reinterpret_cast<const char *>(&head), sizeof(head)) &&
           csd.Send(reinterpret_cast<const char *>(&sign), sizeof(sign)) &&
           csd.Send(reinterpret_cast<const char *>(&size), sizeof(size)) &&
           (size ? csd.Send(data, Size()) : true);
}

void Network::Message::Push(u8 byte8)
{
    if(data + dtsz < itd2 + 1) Resize(1);

    *itd2 = byte8;
    ++itd2;
}

void Network::Message::Push(u16 byte16)
{
    if(data + dtsz < itd2 + 2) Resize(2);

    *itd2 = 0x00FF & (byte16 >> 8);
    ++itd2;

    *itd2 = 0x00FF & byte16;
    ++itd2;
}

void Network::Message::Push(u32 byte32)
{
    if(data + dtsz < itd2 + 4) Resize(4);

    *itd2 = 0x000000FF & (byte32 >> 24);
    ++itd2;

    *itd2 = 0x000000FF & (byte32 >> 16);
    ++itd2;

    *itd2 = 0x000000FF & (byte32 >> 8);
    ++itd2;

    *itd2 = 0x000000FF & byte32;
    ++itd2;
}

void Network::Message::Push(const std::string & str)
{
    if(data + dtsz < itd2 + str.size() + 1) Resize(str.size() + 1);

    if(str.size())
    {
	std::string::const_iterator it1 = str.begin();
	std::string::const_iterator it2 = str.end();
	for(; it1 != it2; ++it1, ++itd2) *itd2 = *it1;
    }
    // end string
    *itd2 = 0;
    ++itd2;
}

bool Network::Message::Pop(u8 & byte8)
{
    if(itd1 + 1 > itd2) return false;

    byte8 = *itd1;
    ++itd1;

    return true;
}

bool Network::Message::Pop(u16 & byte16)
{
    if(itd1 + 2 > itd2) return false;

    byte16 = *itd1;
    byte16 <<= 8;
    ++itd1;

    byte16 |= (0x00FF & *itd1);
    ++itd1;

    return true;
}

bool Network::Message::Pop(u32 & byte32)
{
    if(itd1 + 4 > itd2) return false;

    byte32 = *itd1;
    byte32 <<= 8;
    ++itd1;

    byte32 |= (0x000000FF & *itd1);
    byte32 <<= 8;
    ++itd1;

    byte32 |= (0x000000FF & *itd1);
    byte32 <<= 8;
    ++itd1;

    byte32 |= (0x000000FF & *itd1);
    ++itd1;

    return true;
}

bool Network::Message::Pop(std::string & str)
{
    if(itd1 >= itd2) return false;

    // find end string
    const char* end = itd1;
    while(*end && end < itd2) ++end;
    if(end == itd2) return false;

    str = itd1;
    return true;
}

void Network::Message::Dump(std::ostream & stream) const
{
    stream << "Network::Message::Dump: type: 0x" << std::hex << type << ", size: " << std::dec << Size();

    if(dtsz)
    {
	stream << ", data:";
	const char* cur = itd1;
	while(cur < itd2){ stream << " 0x" << std::hex << static_cast<int>(*cur); ++cur; }
    }

    stream  << std::endl;
}

Network::Socket::Socket() : sd(NULL)
{
}

Network::Socket::Socket(const Socket &) : sd(NULL)
{
}

Network::Socket::Socket(const TCPsocket csd) : sd(csd)
{
}

Network::Socket & Network::Socket::operator= (const Socket &)
{
    return *this;
}

Network::Socket::~Socket()
{
    if(sd) Close();
}

u32 Network::Socket::Host(void) const
{
    IPaddress* remoteIP = sd ? SDLNet_TCP_GetPeerAddress(sd) : NULL;
    if(remoteIP) return SDLNet_Read32(&remoteIP->host);

    std::cerr << "Network::Socket::Host: " << GetError() << std::endl;
    return 0;
}

u16 Network::Socket::Port(void) const
{
    IPaddress* remoteIP = sd ? SDLNet_TCP_GetPeerAddress(sd) : NULL;
    if(remoteIP) return SDLNet_Read16(&remoteIP->port);

    std::cerr << "Network::Socket::Host: " << GetError() << std::endl;
    return 0;
}

bool Network::Socket::Recv(char *buf, size_t len) const
{
    if(sd && buf)
    {
	const int rcv = SDLNet_TCP_Recv(sd, buf, len);
	if(rcv > 0) return true;
	std::cerr << "Network::Socket::Recv: size: " << len << ", receive: " << rcv << ", error: " << GetError() << std::endl;
    }
    return false;
}

bool Network::Socket::Send(const char* buf, size_t len) const
{
    return sd && static_cast<int>(len) == SDLNet_TCP_Send(sd, const_cast<void*>(reinterpret_cast<const void*>(buf)), len);
}

bool Network::Socket::Open(IPaddress & ip)
{
    if(sd) Close();
    sd = SDLNet_TCP_Open(&ip);
    if(! sd)
	std::cerr << "Network::Socket::Open: " << Network::GetError() << std::endl;

    return sd;
}

bool Network::Socket::IsValid(void) const
{
    return sd;
}

void Network::Socket::Close(void)
{
    if(sd) SDLNet_TCP_Close(sd);
    sd = NULL;
}

Network::Server::Server()
{
}

TCPsocket Network::Server::Accept(void)
{
    return SDLNet_TCP_Accept(sd);
}

bool Network::Init(void)
{
    if(SDLNet_Init() < 0)
    {
        std::cerr << "Network::Init: " << GetError() << std::endl;
        return false;
    }
    return true;
}

void Network::Quit(void)
{
    SDLNet_Quit();
}

bool Network::ResolveHost(IPaddress & ip, const char* host, u16 port)
{
    if(SDLNet_ResolveHost(&ip, host, port) < 0)
    {
	std::cerr << "Network::ResolveHost: " << GetError() << std::endl;
	return false;
    }
    return true;
}

const char* Network::GetError(void)
{
    return SDLNet_GetError();
}

#endif