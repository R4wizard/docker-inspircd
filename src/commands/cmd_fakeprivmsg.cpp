#include "inspircd.h"

class CommandFakePrivmsg : public Command
{
 public:
        CommandFakePrivmsg ( Module* parent) : Command(parent, "FAKEPRIVMSG", 3, 3) { syntax = "<fakeuser> <target>{,<target>} <message>"; }
        CmdResult Handle(const std::vector<std::string>& parameters, User *user);
};

CmdResult CommandFakePrivmsg::Handle (const std::vector<std::string>& parameters, User *user)
{
        Channel *chan;
        CUList except_list;
        except_list.insert(user);

        char status = 0;

        if (ServerInstance->Parser->LoopCall(user, this, parameters, 0))
            return CMD_SUCCESS;

        if (!user->IsModeSet('B')) {
                user->WriteNumeric(ERR_NOPRIVILEGES, "%s :Your host isn't among the privileged. (bot mode needed)", user->nick.c_str());
                return CMD_FAILURE;
        }

        chan = ServerInstance->FindChan(parameters[1]);

        if (chan)
        {
                const char* text = parameters[2].c_str();
                FakeUser* fakeuser = new FakeUser("000000000", parameters[0]);
                chan->WriteAllExcept(fakeuser, false, status, except_list, "PRIVMSG %s :%s", chan->name.c_str(), text);
                fakeuser->cull();
        }
        else
        {
                user->WriteNumeric(401, "%s %s :No such nick/channel", user->nick.c_str(), parameters[1].c_str());
                return CMD_FAILURE;
        }
        return CMD_SUCCESS;
}

COMMAND_INIT(CommandFakePrivmsg)
