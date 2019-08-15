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

	while (true)
	{
		try
		{
			responseBody = client.GetStringAsync({ L"https://api.coinbase.com/v2/prices/spot?currency=USD" }).get();
			JsonObject jsonObject = JsonObject::Parse(responseBody);
			IJsonValue dataValue = jsonObject.Lookup(L"data");
			hstring s = dataValue.Stringify();
			JsonObject dataObject = JsonObject::Parse(s);
			hstring amountValue = dataObject.GetNamedString(L"amount");
			std::wcout << "BTCUSD $" << amountValue.c_str() << '\n';
			sleep_for(31s);
		}
		catch (hresult_error const& ex)
		{
			responseBody = ex.message();
		}
	}
}
