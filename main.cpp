#include "pch.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Web::Http;
using namespace Windows::Data::Json;
using namespace std::chrono_literals;
using namespace std::this_thread;

int main()
{
    init_apartment();
	HttpClient client;
	hstring responseBody;
	hstring oldAmount;

	while (true)
	{
		try
		{
			responseBody = client.GetStringAsync({ L"https://api.coinbase.com/v2/prices/spot?currency=USD" }).get();
			JsonObject jsonObject = JsonObject::Parse(responseBody);
			IJsonValue dataValue = jsonObject.Lookup(L"data");
			hstring s = dataValue.Stringify();
			JsonObject dataObject = JsonObject::Parse(s);
			hstring newAmount = dataObject.GetNamedString(L"amount");
			_setmode(_fileno(stdout), _O_U16TEXT);
			if (newAmount >= oldAmount)
			{
				std::wcout << L"BTCUSD ↑ $" << L"\033[92m" << newAmount.c_str() << L"\033[0m" << '\n';
			}
			else
			{
				std::wcout << L"BTCUSD ↓ $" << L"\033[91m" << newAmount.c_str() << L"\033[0m" << '\n';
			}
			oldAmount = newAmount;
			sleep_for(31s);
		}
		catch (hresult_error const& ex)
		{
			responseBody = ex.message();
		}
	}
}
