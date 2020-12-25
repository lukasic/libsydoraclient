#include <iostream>

#include <EPP/EppClient.h>
#include <Sydora/Sydora.h>

#include <Sydora/Poll/TransferData.h>

void poll(EPP::Client<Sydora::Sydora>& client)
{
	try
	{
		int cnt = client->getPollMessagesCount();
		std::cout << "Poll messages count: " << cnt << std::endl;

		while (cnt-- > 0)
		{
			PollData::Ptr dataInstance = client->getPollMessage();

			if (dataInstance->getClassName() == "TransferData")
			{
				TransferData* trnData = (TransferData*) dataInstance.get();

				std::cout << "Message.id: " << trnData->msgID << std::endl;

				std::string domainName = trnData->name;

				client->ackPollMessage(trnData->msgID);
			}
		}

	}

	catch (EPP::Exception& e)
	{
		std::cout << e;
	}

}

