#include "IrcServer.hpp"
#include <algorithm>

int IrcServer::names(User &u, const Message &m)
{

	//===========TO-DO===========//
	// Gerer les targets
	//===========TO-DO===========//


	const Network::ChannelMap &channels = network.channels();

	if (!m.params().size())
	{
		
		Network::ChannelMap::const_iterator ic = channels.begin();
		while (ic != channels.end())
		{
			std::string listName = "";
		
			if (ic->second->nbUserVisible() > 0 || ic->second->findMember(&u))
			{

				ChannelMode cmode = ic->second->mode();
				if (cmode.isSet(ChannelMode::SECRET))
					listName += "@ " + ic->first + " :";
				else if (!cmode.isSet(ChannelMode::SECRET) && !cmode.isSet(ChannelMode::PRIVATE))
					listName += "= " + ic->first + " :";
				else
					listName += "* " + ic->first + " :";
				const Channel::MemberMap &members = ic->second->members();
	        	Channel::MemberMap::const_iterator im = members.begin();
	        	while (im != members.end())
	        	{
					MemberMode *mmode = ic->second->findMember(im->first);
					if (mmode != NULL)
					{
						if (mmode->isSet(MemberMode::OPERATOR) || mmode->isSet(MemberMode::CREATOR))
	        				listName += "@" + im->first->nickname() + (++im == members.end()-- ? "" : " ");
						else if (mmode->isSet(MemberMode::VOICE) && (!mmode->isSet(MemberMode::OPERATOR)
						&& !mmode->isSet(MemberMode::CREATOR)))
	        				listName += "+" + im->first->nickname() + (++im == members.end()-- ? "" : " ");
						else
	        				listName += im->first->nickname() + (++im == members.end()-- ? "" : " ");
					}
					else
	        			listName += im->first->nickname() + (++im == members.end()-- ? "" : " ");
	        	}
	   			writeNum(u, IrcReply::namreply(listName));
			}
			++ic;
		}
		writeNum(u, IrcReply::endofnames("*"));
	}
	else
	{
		std::vector<Param> paramList= m.params()[0].split();

		std::vector<Param>::const_iterator ip = paramList.begin();
		while (ip != paramList.end())
		{
			
			std::string listName = "";
	
			//--------------- FIND -------------------//	
			short int paramInChannels = 0;		
			Network::ChannelMap::const_iterator ic = channels.begin();
			while (ic != channels.end())
			{
				if (ic->first == (*ip))
				{
					paramInChannels = 1;
					break;
				}
				++ic;
			}
			//---------------------------------------//	
			
			if (paramInChannels)
			{

				if (ic->second->nbUserVisible() > 0 || ic->second->findMember(&u))
				{
					ChannelMode cmode = ic->second->mode();
					if (cmode.isSet(ChannelMode::SECRET))
						listName += "@ " + (*ip) + " :";
					else if (!cmode.isSet(ChannelMode::SECRET) && !cmode.isSet(ChannelMode::PRIVATE))
						listName += "= " + (*ip) + " :";
					else
						listName += "* " + (*ip) + " :";
					const Channel::MemberMap &members = ic->second->members();
		            Channel::MemberMap::const_iterator im = members.begin();
		            while (im != members.end())
		            {
						MemberMode *mmode = ic->second->findMember(im->first);
						if (mmode != NULL)
						{
							if (mmode->isSet(MemberMode::OPERATOR) || mmode->isSet(MemberMode::CREATOR))
		        				listName += "@" + im->first->nickname() + (++im == members.end()-- ? "" : " ");
							else if (mmode->isSet(MemberMode::VOICE) && (!mmode->isSet(MemberMode::OPERATOR)
							&& !mmode->isSet(MemberMode::CREATOR)))
		        				listName += "+" + im->first->nickname() + (++im == members.end()-- ? "" : " ");
							else
		        				listName += im->first->nickname() + (++im == members.end()-- ? "" : " ");
						}
						else
		        			listName += im->first->nickname() + (++im == members.end()-- ? "" : " ");
	            	}
				}
	    		writeNum(u, IrcReply::namreply(listName));
				writeNum(u, IrcReply::endofnames((*ip)));
			}
			++ip;
		}
	}
    return (0);
}

/*
> Command : NAMES
> params : <channel> <target>
*/
