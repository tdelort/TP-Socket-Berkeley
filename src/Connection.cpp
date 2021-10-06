#include "Connection.h"

unsigned int Connection::newID = 0;

Connection::Connection()
	: m_id(newID++)
{}

void Connection::AddConfig(Config config) 
{
	m_config = config;
}

unsigned int Connection::GetID()
{
	return m_id;
}