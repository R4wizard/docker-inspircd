#include "inspircd.h"

class CommandFakeJoin : public Command
{
 public:
        CommandFakeJoin ( Module* parent) : Command(parent, "FAKEJOIN", 2, 2) { syntax = "<fakeuser> <channel>{,<channel>}"; }
        CmdResult Handle(const std::vector<std::string>& parameters, User *user);
};

CmdResult CommandFakeJoin::Handle (const std::vector<std::string>& parameters, User *user)
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
                FakeUser* fakeuser = new FakeUser("000000000", parameters[0]);
                chan->WriteAllExcept(fakeuser, false, status, except_list, "JOIN %s", chan->name.c_str());
                fakeuser->cull();
        }
        else
        {
                user->WriteNumeric(401, "%s %s :No such nick/channel", user->nick.c_str(), parameters[1].c_str());
                return CMD_FAILURE;
        }
        return CMD_SUCCESS;
}

COMMAND_INIT(CommandFakeJoin)
