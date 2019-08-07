#include "pch.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Web::Http;
using namespace Windows::Data::Json;

int main()
{
    init_apartment();
	HttpClient httpClient;
	Uri requestUri{ L"https://api.coinbase.com/v2/prices/spot?currency=USD" };
	HttpResponseMessage httpResponseMessage;
	hstring httpResponseBody;
	try
	{
		httpResponseMessage = httpClient.GetAsync(requestUri).get();
		httpResponseMessage.EnsureSuccessStatusCode();
		httpResponseBody = httpResponseMessage.Content().ReadAsStringAsync().get();

	}
	catch (hresult_error const& ex)
	{
		httpResponseBody = ex.message();
	}

	JsonObject jsonObject = JsonObject();
	bool canParse = JsonObject::TryParse(httpResponseBody, jsonObject);
	bool hasDataKey = jsonObject.HasKey(L"data"); // Debug, look for "data" key at the beginning
	IJsonValue dataValue = jsonObject.Lookup(L"data");
	hstring s = dataValue.Stringify();

	JsonObject dataObject = JsonObject();
	bool canParseAgain = JsonObject::TryParse(s, dataObject);
	bool hasAmountKey = dataObject.HasKey(L"amount"); // Debug
	hstring amountValue = dataObject.GetNamedString(L"amount");

	//std::wcout << httpResponseBody.c_str() << std::endl;
	std::wcout << "$" << amountValue.c_str();
}
