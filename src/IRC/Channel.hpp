#pragma once
#include <map>
#include <set>
#include "User.hpp"

class MemberMode : public Mode
{
public:
	enum Flag
	{
		/** 'O' - Channel creator status */
		CREATOR = 1,
		/** 'o' - Channel operator privilege */
		OPERATOR = 1 << 2,
		/** 'v' - Voice privilege */
		VOICE = 1 << 3
	};

	MemberMode(unsigned flags = 0);
	~MemberMode();

	bool canTalk() const;

	static Flag	parse(char c);
};

class ChannelMode : public Mode
{
public:
	enum Flag
	{
		/** 'i' - Invite-only Channel flag */
		INVITE_ONLY = 1 << 1,
		/** 'm' - Moderated Channel flag */
		MODERATED = 1 << 2,
		/** 'n' - No IRC::Messages to Channel from User on the outside */
		NO_OUTSIDE_MSG = 1 << 3,
		/** 'p' - Private channel flag */
		PRIVATE = 1 << 5,
		/** 's' - Secret channel flag */
		SECRET = 1 << 6,
		/** 't' - Topic settable by Channel operator only flag */
		TOPIC_SETTABLE_BY_CHANOP = 1 << 8,
		/** 'k' - Channel key */
		KEY = 1 << 9,
		/** 'l' - User limit */
		LIMIT = 1 << 10,
		/** 'b' - Ban mask */
		BAN_MASK = 1 << 11,
		/** 'e' - Exception mask */
		EXCEPTION_MASK = 1 << 12,
		/** 'I' - Invitation mask */
		INVITATION_MASK = 1 << 13
	};

	ChannelMode(unsigned flags = 0);
	~ChannelMode();

	static Flag	parse(char c);

	std::string	toString() const;

private:
	/** Quick lookup table for lower alphabet */
	static const unsigned short _lowerFlagTable[26];
};

class MaskSet : public std::set<std::string>
{
public:
	MaskSet();
	~MaskSet();

	bool inSet(const User &user) const;
};

class Channel
{
public:
	typedef std::set<std::string>		InvitationSet;
	typedef std::map<User*, MemberMode>	MemberMap;

	enum Type
	{
		/** '#' */
		GLOBAL,
		/** '+' */
		UNMODERATED
	};

	enum MaskSetType
	{
		BAN_SET,
		EXCEPTION_SET,
		INVITATION_SET
	};

	Channel(const std::string &name = "");
	~Channel();

	const std::string	&name() const;
	Type				type() const;
	const ChannelMode	&mode() const;
	const MemberMap		&members() const;
	const std::string	&topic() const;
	const std::string	&key() const;
	size_t				limit() const;
	const MaskSet		&masks(const MaskSetType type) const;
	void				setMode(const ChannelMode &mode);
	void				setTopic(const std::string &topic);
	void				setKey(const std::string &key);
	void				setLimit(size_t limit);
	size_t				count() const;
	bool				isBanned(const User *user) const;
	bool				isInvited(const User *user) const;
	size_t				nbUserVisible() const;
	void				send(const std::string &msg, BasicConnection *origin = NULL, bool useReceipt = false) const;
	void				addMember(User *user, const MemberMode &mode);
	void				addMask(const MaskSetType type, const std::string &mask);
	bool				canSendToChannel(User *user);
	void				delMember(User *user);
	void				delMask(const MaskSetType type, const std::string &mask);
	MemberMode			*findMember(User *user);
	void				invite(const User *user);
	void				markAllMembers();

private:
	const std::string	_name;
	const Type			_type;
	ChannelMode			_mode;
	MemberMap			_members;
	std::string			_topic;
	std::string			_key;
	size_t				_limit;
	MaskSet				_masks[3];
	InvitationSet		_invitations;
};
