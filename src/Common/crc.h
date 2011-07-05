// forward references
class CRC;
class CRC_DETAILS;

class CRC
{
public:
   enum TYPE
   {
      CRC_16,
      CRC_32,
   };
   CRC(TYPE eType);
   ~CRC();
   void        Initialize();
   void        Update (const void *pvData, int iLength);
   unsigned long  Value () const;
private:
   CRC_DETAILS *m_pDetails;
};
