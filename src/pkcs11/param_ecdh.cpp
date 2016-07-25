#include "param_ecdh.h"

ParamEcdh1::ParamEcdh1() {
	New();
}

ParamEcdh1::~ParamEcdh1() {
	Free();
}

void ParamEcdh1::FromV8(Local<Value> v8Value) {
	try {
		Nan::HandleScope();

		if (!v8Value->IsObject()) {
			THROW_ERROR("Parameter 1 MUST be Object", NULL);
		}

		Local<Object> v8Params = v8Value->ToObject();

		// Check data
		if (!v8Params->Get(Nan::New(STR_KDF).ToLocalChecked())->IsNumber())
			THROW_ERROR("Attribute 'kdf' MUST be NUMBER", NULL);
		if (!(v8Params->Get(Nan::New(STR_SHARED_DATA).ToLocalChecked())->IsUndefined()
			|| node::Buffer::HasInstance(v8Params->Get(Nan::New(STR_SHARED_DATA).ToLocalChecked())->ToObject())))
			THROW_ERROR("Attribute 'sharedData' MUST be NULL | Buffer", NULL);
		if (!node::Buffer::HasInstance(v8Params->Get(Nan::New(STR_PUBLIC_DATA).ToLocalChecked())->ToObject()))
			THROW_ERROR("Attribute 'publicData' MUST be Buffer", NULL);

		Free();
		New();

		data.kdf = (CK_ULONG)v8Params->Get(Nan::New(STR_KDF).ToLocalChecked())->ToNumber()->Uint32Value();

		if (!v8Params->Get(Nan::New(STR_SHARED_DATA).ToLocalChecked())->IsUndefined()) {
			GET_BUFFER_SMPL(sharedData, v8Params->Get(Nan::New(STR_SHARED_DATA).ToLocalChecked())->ToObject());
			data.pSharedData = (CK_BYTE_PTR)malloc(sharedDataLen * sizeof(CK_BYTE));
			memcpy(data.pSharedData, sharedData, sharedDataLen);
			data.ulSharedDataLen = (CK_ULONG) sharedDataLen;
		}

		GET_BUFFER_SMPL(publicData, v8Params->Get(Nan::New(STR_PUBLIC_DATA).ToLocalChecked())->ToObject());
		data.pPublicData = (CK_BYTE_PTR)malloc(publicDataLen * sizeof(CK_BYTE));
		memcpy(data.pPublicData, publicData, publicDataLen);
		data.ulPublicDataLen = (CK_ULONG) publicDataLen;
	}
	CATCH_ERROR;
}

CK_ECDH1_DERIVE_PARAMS_PTR ParamEcdh1::New() {
	data = CK_ECDH1_DERIVE_PARAMS();
	data.kdf = CKD_NULL;
	data.pSharedData = NULL;
	data.ulSharedDataLen = 0;
	data.pPublicData = NULL;
	data.ulPublicDataLen = 0;

	return &data;
}

void ParamEcdh1::Free() {
	if (&data) {
		if (data.pSharedData)
			free(data.pSharedData);
		if (data.pPublicData)
			free(data.pPublicData);
	}
}