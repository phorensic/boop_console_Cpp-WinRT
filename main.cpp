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
	HttpClient httpClient;
	Uri requestUri{ L"https://api.coinbase.com/v2/prices/spot?currency=USD" };
	HttpResponseMessage httpResponseMessage;
	hstring httpResponseBody;

	while (true)
	{
		try
		{
			httpResponseMessage = httpClient.GetAsync(requestUri).get();
			httpResponseBody = httpResponseMessage.Content().ReadAsStringAsync().get();

			JsonObject jsonObject = JsonObject::Parse(httpResponseBody);
			IJsonValue dataValue = jsonObject.Lookup(L"data");
			hstring s = dataValue.Stringify();
			JsonObject dataObject = JsonObject::Parse(s);
			hstring amountValue = dataObject.GetNamedString(L"amount");

			std::wcout << "BTCUSD $" << amountValue.c_str() << '\n';
			sleep_for(31s);
		}
		catch (hresult_error const& ex)
		{
			httpResponseBody = ex.message();
		}

		
	}
}
